/*
 * @Author       : FeiYehua
 * @Date         : 2025-08-11 11:07:02
 * @LastEditTime : 2025-08-11 16:15:03
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : builtin.c
 *      © 2024 FeiYehua
 */

#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include "csapp.h"
#include "builtin.h"
#include "signal_handers.h"
#include "jobs.h"

#define MAXLINE 1024 /* max line size */
#define MAXARGS 128  /* max args on a command line */

/*
 * parseline - Parse the command line and build the argv array.
 *
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.
 */
int parseline(const char *cmdline, char **argv)
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf) - 1] = ' ';   /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'')
    {
        buf++;
        delim = strchr(buf, '\'');
    }
    else
    {
        delim = strchr(buf, ' ');
    }

    while (delim)
    {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* ignore spaces */
            buf++;

        if (*buf == '\'')
        {
            buf++;
            delim = strchr(buf, '\'');
        }
        else
        {
            delim = strchr(buf, ' ');
        }
    }
    argv[argc] = NULL;

    if (argc == 0) /* ignore blank line */
        return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc - 1] == '&')) != 0)
    {
        argv[--argc] = NULL;
    }
    return bg;
}

/*
 * eval - Evaluate the command line that the user has just typed in
 *
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.
 */
void eval(char *cmdline)
{
    char **argv = malloc(sizeof(char *) * MAXARGS);
    // deterimine if the command need to be executed in foreground or background
    // bg = 1: background or empty line
    // bg = 0: foreground
    int bg = parseline(cmdline, argv);
    if (argv[0] == NULL)
    {
        // Empty line
        free(argv);
        return;
    }
    int isBuiltin = builtin_cmd(argv);
    if (!isBuiltin) // Not a builtin command
    {
        // fork and execve
        pid_t pid = Fork();
        Signal(SIGCHLD, SIG_IGN);
        sigset_t mask_sigchld, prev_mask, mask_all;
        Sigaddset(&mask_sigchld, SIGCHLD);
        Sigfillset(&mask_all);
        Sigprocmask(SIG_BLOCK, &mask_sigchld, &prev_mask); // Block SIGCHLD in process
        if (pid == 0)
        {
            // Child process
            Setpgid(0, 0);                              // Create a new process group
            Sigprocmask(SIG_SETMASK, &prev_mask, NULL); // Childprocess should have normal signal handler
            Execve(argv[0], argv, NULL);                // Execute command
        }
        else
        {
            if (!bg)
            {
                // Not a background task, wait for the process to terminate
                Sigprocmask(SIG_BLOCK, &mask_all, NULL);    // Block all signals for job operations
                addjob(jobs, pid, FG, cmdline);             // Add job as a foreground task
                Sigprocmask(SIG_SETMASK, &prev_mask, NULL); // Unblock SIGCHLD
                waitfg(pid);                                // Wait until foreground task finishes
            }
            else
            {
                Sigprocmask(SIG_BLOCK, &mask_all, NULL); // Block all signals for job operations
                addjob(jobs, pid, BG, cmdline);          // Add job as a background task
                char *line = malloc(MAXLINE * sizeof(char));
                sprintf(line, "[%d] (%d) %s", pid2jid(pid), pid, cmdline);
                Sio_puts(line);                             // Print job info
                free(line);                                 // Free used space
                Sigprocmask(SIG_SETMASK, &prev_mask, NULL); // Unblock SIGCHLD
            }
        }
    }
    free(argv); // Free memory of argv
    return;
}

/*
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.
 */
int builtin_cmd(char **argv)
{
    if (strcmp(argv[0], "quit") == 0)
    {
        // quit command
        exit(0);
    }
    if (strcmp(argv[0], "jobs") == 0)
    {
        listjobs(jobs);
        return 1;
    }
    if (strcmp(argv[0], "bg") == 0 || strcmp(argv[0], "fg") == 0)
    {
        do_bgfg(argv);
        return 1;
    }
    return 0; /* not a builtin command */
}

/*
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv)
{
    return;
}

/*
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
    int status = 0;
    Waitpid(pid, &status, WUNTRACED);
    return;
}