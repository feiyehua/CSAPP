/***
 * @Author       : FeiYehua
 * @Date         : 2025-08-11 10:50:56
 * @LastEditTime : 2025-08-11 10:53:23
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : wrapper.h
 * @     © 2024 FeiYehua
 */

#pragma once

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* Type definition for signal handler */
typedef void handler_t(int);

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler)
{
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return (old_action.sa_handler);
}