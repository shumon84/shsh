/**
 * @file shell.h
 *
 * Copyright(c) 2017 藤田朱門
 *
 * This software is released under the MIT License.
 * http;//opensource.org/licenses/MIT
 *
 * @brief shell.cのヘッダファイル
 * @author 藤田朱門
 * @date 2017/11/6
 */
#ifndef SHELL_H_
#define BUFS (1024)		/* 入力できるコマンドの最大の文字数 */
#define ARGS (1024)		/* 許容する最大の引数の数 */
#define PRCS (1024)		/* 同時に実行できる最大のプロセス数 */

typedef struct Prc		/* プロセス情報を持つ構造体 */
{
  pid_t pid;			/* プロセスID */
  char name[BUFS];		/* プロセス名 */
}prc;

void argdiv(char *arg[],char *str);
void wait_input(char *arg[]);
void wait_input(char *arg[]);
void bg_end(pid_t endid);
void child(char *cmd,char *arg[],char *envp[]);
void parent(pid_t pid,char *cmd);
#endif
