/**
 * @file shell.c
 *
 * Copyright(c) 2017 藤田朱門
 *
 * This software is released under the MIT License.
 * http;//opensource.org/licenses/MIT
 *
 * @brief シェルの内部処理
 * @author 藤田朱門
 * @date 2017/11/6
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<err.h>

#include<shell.h>
#include<builtin.h>

bool flg_bg;		/* BGプロセスのフラグ */
process prcs[PRCS];     /* 実行中のプロセスのPIDリスト */
int prcn=0;		/* 実行中のプロセスの数 */
pid_t fg_pid;		/* 実行中のFGプロセスのPID */

/**
 * @brief 受け取った文字列を走査して引数に分割
 *
 * 引数の分割時にBGプロセスのフラグ処理も同時に行う
 *
 * @param[in,out] arg 分割された引数の配列
 * @param[in,out] str 分割前の文字列
 * @param[out] argnum 分割された引数の配列の要素数
 */
int argdiv(char *arg[],char *str)
{
  int i,argnum=0;
  flg_bg=false;			/* フラグの初期化 */
  arg[argnum++]=str;		/* 第一引数をセット */
  for(i=0;str[i]!='\0';i++)
    {
      if(str[i]==' ')		/* 空白を見つけたとき */
	{
	  str[i]='\0';		/* 文字列を分割 */
	  while(str[++i]==' '); /* 連続する空白を読み飛ばす */
	  if(str[i]=='&')	/* BGプロセスとして起動する場合 */
	    flg_bg=true;
	  else
	    arg[argnum++]=&str[i];	/* 次の引数に代入 */
	}
    }
  return argnum;
}

/**
 * @brief コマンドの入力を待つ
 *
 * @param[in,out] arg 入力されたコマンド
 * @param[out] argnum コマンドの引数の数
 */
int wait_input(char *arg[])
{
  static char str[BUFS];
  int argnum;
  int i;
  for(i=0;i<BUFS;i++)
    str[i]='\0';
  
  /* プロンプトを表示して入力を受け取る */
  printf("$ ");
  fgets(str,BUFS,stdin);

  str[strlen(str)-1]='\0';	/* 改行を消去 */

  argnum=argdiv(arg,str);	/* コマンドを引数ごとに分割 */

  return argnum;
}

/**
 * @brief BG実行中の子プロセスが終了したときの処理
 *
 * @param[in] endid 終了した子プロセスのPID
 */
void bg_end(pid_t endid)
{
  if(endid<=0)
    return;
  
  int i,j;
  for(i=j=0;i<prcn;i++)		/* 終了したPIDをもつプロセスを探す */
    if(prcs[i].pid!=endid)
      prcs[j++]=prcs[i];	/* 終了したPIDを消して1つシフト */
    else
      fprintf(stderr,"[%d] Done\t%s\n",prcs[i].pid,prcs[i].name);      
  prcn--;			/* 実行中のプロセスの数を減らす */
}

/**
 * @brief 子プロセスでの挙動
 *
 * @param[in] cmd コマンド
 * @param[in] arg 引数配列
 * @param[in] envp 環境変数
 */
void child(char *cmd,char *arg[],char *envp[])
{
  execve(cmd,arg,envp);		/* コマンドを実行 */
  exit(0);
}

/**
 * @brief 親プロセスでの挙動
 *
 * @param[in] pid 生成した子プロセスのプロセスID
 * @param[in] cmd コマンド
 */
void parent(pid_t pid,char *cmd)
{
  int status;

  if(flg_bg==false)		/* FGプロセスの場合 */
    {
      fg_pid=pid;
      waitpid(pid,&status,0);
      fg_pid=-1;
    }
  else				/* BGプロセスの場合 */
    {
      int i;
      for(i=0;cmd[i]!='\0';i++)	/* プロセス名を登録 */
	prcs[prcn].name[i]=cmd[i];
      prcs[prcn].name[i]='\0';	/* 以前登録されたプロセス名との干渉を防ぐ */
      prcs[prcn].pid=pid;	/* PIDを登録 */
      prcn++;
    }
}
