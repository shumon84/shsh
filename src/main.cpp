/**
 * @file main.c
 *
 * Copyright(c) 2017 藤田朱門
 *
 * This software is released under the MIT License.
 * http;//opensource.org/licenses/MIT
 *
 * @brief mainループ
 * @author 藤田朱門
 * @date 2017/11/25
 */
#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include<unistd.h>

#include<shell.h>
#include<builtin.h>

int main(int argc,char *argv[],char *envp[])
{
  pid_t pid;			/* folkした子プロセスのPID */
  
  while(1)
    {
      char *arg[ARGS]={};

      wait_input(arg);		/* 入力を受け取る */
      builtin(arg[0],arg,envp); /*ビルトインコマンドの実行 */

      pid=fork();		/* プロセスをフォーク */

      switch(pid)
	{
	case -1:perror("folk"); continue;     /* folkに失敗した場合 */
	case 0:child(arg[0],arg,envp); break; /* 子プロセス */
	default:parent(pid,arg[0]); break;    /* 親プロセス */
	}
    }

  return 0;
}
