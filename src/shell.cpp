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

/**
 * @brief 受け取った文字列を走査して引数に分割
 *
 * 引数の分割時にBGプロセスのフラグ処理も同時に行う
 *
 * @param[in,out] arg 分割された引数の配列
 * @param[in,out] str 分割前の文字列
 */
void argdiv(char *arg[],char *str)
{
  int i,j=0;
  flg_bg=false;			/* フラグの初期化 */
  arg[0]=str;			/* 第一引数をセット */
  for(i=0;str[i]!='\0';i++)
    {
      if(str[i]==' ')		/* 空白を見つけたとき */
	{
	  str[i]='\0';		/* 文字列を分割 */
	  while(str[++i]==' '); /* 連続する空白を読み飛ばす */
	  if(str[i]=='&')	/* BGプロセスとして起動する場合 */
	    flg_bg=true;
	  else
	    arg[++j]=&str[i];	/* 次の引数に代入 */
	}
    }
}

/**
 * @brief コマンドの入力を待つ
 *
 * @param[in,out] arg 入力されたコマンド
 */
void wait_input(char *arg[])
{
  static char str[BUFS];
  int i;
  for(i=0;i<BUFS;i++)
    str[i]='\0';
  
  /* プロンプトを表示して入力を受け取る */
  printf("$ ");
  fgets(str,BUFS,stdin);

  str[strlen(str)-1]='\0';	/* 改行を消去 */

  argdiv(arg,str);		/* コマンドを引数ごとに分割 */
}

/**
 * @brief BG実行中の子プロセスが終了したときの処理
 *
 * @param[in] endid 終了した子プロセスのPID
 */
void bg_end(pid_t endid)
{
  int i,j;
  for(i=j=0;i<prcn;i++)		/* 終了したPIDをもつプロセスを探す */
    if(prcs[i].pid!=endid)
      prcs[j++]=prcs[i];	/* 終了したPIDを消して1つシフト */
  prcn--;			/* 実行中のプロセスの数を減らす */
  fprintf(stderr,"[%d] terminated\n",endid);
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
  pid_t endid;
  int status;

  if(flg_bg==false)		/* FGプロセスの場合 */
    {
      waitpid(pid,&status,0);
    }
  else
    {
      int i;
      for(i=0;cmd[i]!='\0';i++)	/* プロセス名を登録 */
	prcs[prcn].name[i]=cmd[i];
      prcs[prcn].name[i]='\0';	/* 以前登録されたプロセス名との干渉を防ぐ */
      prcs[prcn].pid=pid;	/* PIDを登録 */
      prcn++;
    }

  //  for(int i=0;i<prcn;i++)printf("debug [%d] = %s\n",prcs[i].pid,prcs[i].name); // debug
  endid=waitpid(-1,&status,WNOHANG);
  //printf("debug endid = %d\n",endid); // debug

  switch(endid)			/* どのプロセスが終了したか確認 */
    {
    case 0:break;		/* どのプロセスも終了していない場合 */
    case -1:break;
    default:bg_end(endid); break; /* プロセスが終了した場合 */
    }
}
