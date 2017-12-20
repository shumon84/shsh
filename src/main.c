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
#include<path.h>

void setup() __attribute__((constructor));

int main(int argc,char *argv[],char *envp[])
{
  extern pid_t fg_pid;
  while(1)
    {
      char *arg[ARGS]={};	/* 引数配列 */
      int argnum;		/* 引数の数 */
      pid_t pid;		/* folkした子プロセスのPID */
      char cmd[BUFS];

      fg_pid=-1;		/* FGプロセスがないので、-1を設定 */

      argnum=wait_input(arg);	/* 入力を受け取る */

      builtin(argnum,arg,envp); /* ビルトインコマンドの実行 */

      if(path_search(arg[0],cmd)!=NULL)
	{      
	  pid=fork();		/* プロセスをフォーク */
	  switch(pid)
	    {
	    case -1:perror("folk"); break; /* folkに失敗した場合 */
	    case 0:child(cmd,arg,envp); break; /* 子プロセス */
	    default:parent(pid,arg[0]); break;    /* 親プロセス */
	    }
	}
      else
	{
	  fprintf(stderr,"%s: command not found\n",arg[0]);
	}
    }
  return 0;
}

/**
 * @brief 前処理を行う
 *
 * main関数よりも先に実行される
 */
void setup()
{
  signal_set();
  path_set();
}
