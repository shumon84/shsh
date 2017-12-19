/**
 * @file builtin.h
 *
 * Copyright(c) 2017 藤田朱門
 *
 * This software is released under the MIT License.
 * http;//opensource.org/licenses/MIT
 *
 * @brief builtin.cのヘッダファイル
 * @author 藤田朱門
 * @date 2017/11/6
 */
#ifndef BUILTIN_H_
#define BUILTIN_H_

#include<stdbool.h>

bool builtin_exit(int argnum,char *arg[],char *envp[]);
bool builtin_quit(int argnum,char *arg[],char *envp[]);
bool builtin_jobs(int argnum,char *arg[],char *envp[]);
bool builtin_fg(int argnum,char *arg[],char *envp[]);
void builtin(int argnum,char *arg[],char *envp[]);
#endif
