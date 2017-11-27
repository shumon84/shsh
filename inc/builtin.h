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

void builtin_exit(char *cmd,char *arg[],char *envp[]);
void builtin_quit(char *cmd,char *arg[],char *envp[]);
void builtin(char *cmd,char *arg[],char *envp[]);
#endif
