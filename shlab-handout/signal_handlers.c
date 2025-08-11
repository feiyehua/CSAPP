/*
 * @Author       : FeiYehua
 * @Date         : 2025-08-11 10:56:21
 * @LastEditTime : 2025-08-11 17:27:23
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : signal_handlers.c
 *      © 2024 FeiYehua
 */
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "jobs.h"
#include "stdio.h"
#include "csapp.h"
extern int verbose;
/*
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.
 */
void sigchld_handler(int sig)
{
    int olderrno = errno;
    pid_t pid;
    sigset_t mask, prev_mask;
    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);
    int status;
    // Reap zombie processes, and modify status in jobs
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0)
    {
        if (WIFSTOPPED(status)) // The job is stopped because of a ctrl-z
        {
            struct job_t *job = getjobpid(jobs, pid);
            job->state = ST;
            if (verbose)
            {
                char *buf = malloc(MAXLINE * sizeof(char));
                sprintf(buf, "sigchld_handler: Job (%d) stopped", pid);
                Sio_puts(buf);
                free(buf);
            }
        }
        else // The process is terminated
        {
            deletejob(jobs, pid);
            if (verbose)
            {
                char *buf = malloc(MAXLINE * sizeof(char));
                sprintf(buf, "sigchld_handler: Job (%d) deleted", pid);
                Sio_puts(buf);
                free(buf);
            }
        }
    }
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL);
    errno = olderrno;
    return;
}

/*
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.
 */
void sigint_handler(int sig)
{
    int olderrno = errno;
    sigset_t mask, prev_mask;
    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask); // Block all new-coming signals
    pid_t pid = fgpid(jobs); // Get the foreground job pid
    if (pid != 0)
    {
        Kill(pid, SIGINT); // Sent SIGINT to current foreground job (and the other processes in the process group)
        char *buf = malloc(MAXLINE * sizeof(char));
        sprintf(buf, "Job [%d] (%d) terminated by signal 2\n", pid2jid(pid), pid);
        Sio_puts(buf);
        free(buf);
    }
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL);
    errno = olderrno;
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.
 */
void sigtstp_handler(int sig)
{
    int olderrno = errno;
    sigset_t mask, prev_mask;
    Sigfillset(&mask);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask); // Block all new-coming signals
    pid_t pid = fgpid(jobs);                   // Get the foreground job pid
    if (pid != 0)
    {
        Kill(-pid, SIGTSTP); // Sent SIGTSTP to current foreground job (and the other processes in the process group)
        char *buf = malloc(MAXLINE * sizeof(char));
        sprintf(buf, "Job [%d] (%d) stopped by signal 20\n", pid2jid(pid), pid);
        Sio_puts(buf);
        free(buf);
    }
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL);
    errno = olderrno;
    return;
}
