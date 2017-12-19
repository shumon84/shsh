/**
 * @file builtin.c
 *
 * Copyright(c) 2017 藤田朱門
 *
 * This software is released under the MIT License.
 * http;//opensource.org/licenses/MIT
 *
 * @brief シグナルハンドラ関数の実装
 * @author 藤田朱門
 * @date 2017/11/6
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

/**
 * @brief FGプロセスを終了させるシグナルハンドラ
 *
 * SIGINTの処理
 */
void signal_quit(int signum)
{
  extern pid_t fg_pid;		/* 実行中のFGプロセスのPID */
  if(fg_pid!=-1)		/* 実行中のFGプロセスがあるか確認 */
    kill(fg_pid,SIGINT);	/* 実行中のFGプロセスを終了させる */
}

/**
 * @brief 終了したプロセスの後処理をするシグナルハンドラ
 *
 * SIGCHLDの処理
 */
#include<shell.h>
#include<sys/wait.h>
void signal_status(int signum)
{
  int status;
  bg_end(waitpid(-1,&status,WNOHANG)); /* 終了したプロセスの後処理 */
}

/**
 * @brief シグナルハンドラの割り当て
 *
 */
typedef struct Sighdl{
  int signum;
  void (*hdl)(int);
}sighdl;
void signal_set() __attribute__((constructor))
{
  int i;
  sighdl handler[]=
    {
      {SIGINT,signal_quit},
      {SIGCHLD,signal_status},
    }; /* シグナルとシグナルハンドラの対応テーブル */
  int hdl_num=sizeof(handler)/sizeof(sighdl);
  for(i=0;i<hdl_num;i++)
    if(signal(handler[i].signum,handler[i].hdl)==SIG_ERR)
      exit(-1);
}
