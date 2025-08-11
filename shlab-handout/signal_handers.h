/*** 
 * @Author       : FeiYehua
 * @Date         : 2025-08-11 10:59:14
 * @LastEditTime : 2025-08-11 10:59:15
 * @LastEditors  : FeiYehua
 * @Description  : 
 * @FilePath     : signal_handers.h
 * @     © 2024 FeiYehua
 */
#pragma once
void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);