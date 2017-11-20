/**
 * @file shell.c
 *
 * Copyright(c) 2017 藤田朱門
 *
 * This software is released under the MIT License.
 * http;//opensource.org/licenses/MIT
 *
 * @brief 自作シェル
 * @author 藤田朱門
 * @date 2017/11/6
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<err.h>

#define true (1)
#define false (0)

#define BUFS (1024)		/* 入力できるコマンドの最大の文字数 */
#define ARGS (1024)		/* 許容する最大の引数の数 */
#define PRCS (1024)		/* 同時に実行できる最大のプロセス数 */

int flg_bg;			/* BGプロセスのフラグ */
int prcs[PRCS];			/* 実行中のプロセスのPIDリスト */
int prcn=0;			/* 実行中のプロセスの数 */

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
  arg[0]=str;
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
  /* プロンプトを表示して入力を受け取る */
  printf("$ ");
  fgets(str,BUFS,stdin);

  str[strlen(str)-1]='\0';	/* 改行を消去 */

  argdiv(arg,str);
}

/**
 * @brief シェルを終了させるビルトインコマンド
 *
 * @param[in] cmd 入力されたコマンド
 */
void exit_shell(char *cmd,char *arg[],char *envp[])
{
  /* 終了コマンド */
  if(strcmp(cmd,"exit")==0 || strcmp(cmd,"quit")==0)
    exit(0);      
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
    if(prcs[i]!=endid)
      prcs[j++]=prcs[i];	/* 終了したPIDを消して1つシフト */
  prcn--;			/* 実行中のバッググラウンドの数を減らす */
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
 */
void parent(pid_t pid)
{
  pid_t endid;
  int status;

  prcs[prcn++]=pid;		/* 実行中のプロセス数を増やしてPIDを登録 */

  for(int i=0;i<prcn;i++)printf("%d\n",prcs[i]); // debug
  
  if(flg_bg==false)		/* FGプロセスの場合 */
    {
      wait(&status);
      prcn--;			/* 実行中のプロセス数を減らす */
    }

  endid=waitpid(-1,&status,WNOHANG);
  printf("endid = %d\n",endid); // debug

  switch(endid)		/* どのプロセスが終了したか確認 */
    {
    case 0:break;
    case -1:perror("waitpid");break;
    default:bg_end(endid); break;
    }
}

int main(int argc,char *argv[],char *envp[])
{
  pid_t pid;
  while(1)
    {
      char *arg[ARGS]={};
      flg_bg=false;

      wait_input(arg);
      exit_shell(arg[0],arg,envp);

      pid=fork();

      switch(pid)
	{
	case -1:perror("folk"); continue;     /* folkに失敗した場合 */
	case 0:child(arg[0],arg,envp); break; /* 子プロセス */
	default:parent(pid); break;    /* 親プロセス */
	}
    }

  return 0;
}
