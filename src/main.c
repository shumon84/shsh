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
#include<signal.h>

#include<shell.h>
#include<builtin.h>
#include<sighdl.h>

int main(int argc,char *argv[],char *envp[])
{
  signal_set();			/* シグナルハンドラの設定 */
  
  while(1)
    {
      char *arg[ARGS]={};	/* 引数配列 */
      int argnum;		/* 引数の数 */
      pid_t pid;		/* folkした子プロセスのPID */

      argnum=wait_input(arg);	/* 入力を受け取る */
      builtin(argnum,arg,envp); /* ビルトインコマンドの実行 */
      pid=fork();		/* プロセスをフォーク */
      switch(pid)
	{
	case -1:perror("folk"); break;     /* folkに失敗した場合 */
	case 0:child(arg[0],arg,envp); break; /* 子プロセス */
	default:parent(pid,arg[0]); break;    /* 親プロセス */
	}
    }

  return 0;
}
