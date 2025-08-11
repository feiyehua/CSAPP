/*** 
 * @Author       : FeiYehua
 * @Date         : 2025-08-11 11:10:02
 * @LastEditTime : 2025-08-11 11:10:03
 * @LastEditors  : FeiYehua
 * @Description  : 
 * @FilePath     : builtin.h
 * @     © 2024 FeiYehua
 */
#pragma once
int parseline(const char *cmdline, char **argv);
void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);