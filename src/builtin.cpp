/**
 * @file builtin.c
 *
 * Copyright(c) 2017 藤田朱門
 *
 * This software is released under the MIT License.
 * http;//opensource.org/licenses/MIT
 *
 * @brief 組み込みコマンドの実装
 * @author 藤田朱門
 * @date 2017/11/6
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<errno.h>

/**
 * @brief シェルを終了させる組み込みコマンド
 *
 * @param[in] argnum 入力されたコマンドの引数の数
 * @param[in] arxg    入力されたコマンドの引数
 * @param[in] envp   環境変数
 * @param[out] このコマンドを実行するかどうか
 */
bool builtin_exit(int argnum,char *arg[],char *envp[])
{
  if(strcmp(arg[0],"exit")==0)
    {
      exit(0);
      return true;
    }
  else
    return false;
}

/**
 * @brief シェルを終了させる組み込みコマンド
 *
 * @param[in] argnum 入力されたコマンドの引数の数
 * @param[in] arg    入力されたコマンドの引数
 * @param[in] envp   環境変数
 * @param[out] このコマンドを実行するかどうか
 */
bool builtin_quit(int argnum,char *arg[],char *envp[])
{
  if(strcmp(arg[0],"quit")==0)
    {
      exit(0);
      return true;
    }
  else
    return false;
}

/**
 * @brief 実行中のプロセス一覧を表示する組み込みコマンド
 *
 * @param[in] argnum 入力されたコマンドの引数の数
 * @param[in] arg    入力されたコマンドの引数
 * @param[in] envp   環境変数
 * @param[out] このコマンドを実行するかどうか
 */
#include<shell.h>
bool builtin_jobs(int argnum,char *arg[],char *envp[])
{
  extern process prcs[PRCS];
  extern int prcn;
  if(strcmp(arg[0],"jobs")==0)
    {
      int i;
      for(i=0;i<prcn;i++)	/* 実行中のプロセスを列挙 */
	printf("[%d] %s\n",prcs[i].pid,prcs[i].name);
      return true;
    }
  else
    return false;
}

/**
 * @brief FG実行するプロセスを切り替える組み込みコマンド
 *
 * @param[in] argnum 入力されたコマンドの引数の数
 * @param[in] arg    入力されたコマンドの引数
 * @param[in] envp   環境変数
 * @param[out] このコマンドを実行するかどうか
 */
#include<shell.h>
bool builtin_fg(int argnum,char *arg[],char *envp[])
{
  if(strcmp(arg[0],"fg")==0)
    {
      if(argnum!=2)		/* 引数が少ない、または多すぎる場合 */
      	{
      	  errno=EINVAL;		/* errnoを設定 */
	  perror("fg");
      	}
      else
      	{
	  extern process prcs[PRCS];
	  extern int prcn;
	  int status;
	  pid_t pid=atoi(arg[1]); /* 引数からFGにするPIDを抽出 */
	  if(waitpid(pid,&status,0)!=-1)
	    bg_end(pid);	/* 正常にプロセスが終了した場合 */
	  else
	    perror("fg");	/* 指定されたpidがなかった場合 */
	  }
      return true;
    }
  else
    return false;
}

/**
 * @brief シェル組み込みコマンドを実行する
 *
 * @param[in] argnum 入力されたコマンドの引数の数
 * @param[in] arg    入力されたコマンドの引数
 * @param[in] envp   環境変数
 */
void builtin(int argnum,char *arg[],char *envp[])
{
  static bool (*builtin_command[])(int,char*[],char*[])=
    {
      builtin_exit,
      builtin_quit,
      builtin_jobs,
      builtin_fg,
    }; /* 組み込みコマンドの一覧 */

  int i;
  int n=sizeof(builtin_command)/sizeof(builtin_command[0]);
  for(i=0;i<n;i++) /* 組み込みコマンドが呼ばれているか確認 */
    if(builtin_command[i](argnum,arg,envp))
      break;
}
