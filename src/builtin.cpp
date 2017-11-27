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

/**
 * @brief シェルを終了させる組み込みコマンド
 *
 * @param[in] cmd 入力されたコマンド
 * @param[in] arg 入力されたコマンドの引数
 * @param[in] envp 環境変数
 * @param[out] このコマンドを実行するかどうか
 */
bool builtin_exit(char *cmd,char *arg[],char *envp[])
{
  if(strcmp(cmd,"exit")==0)
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
 * @param[in] cmd 入力されたコマンド
 * @param[in] arg 入力されたコマンドの引数
 * @param[in] envp 環境変数
 * @param[out] このコマンドを実行するかどうか
 */
bool builtin_quit(char *cmd,char *arg[],char *envp[])
{
  if(strcmp(cmd,"quit")==0)
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
 * @param[in] cmd 入力されたコマンド
 * @param[in] arg 入力されたコマンドの引数
 * @param[in] envp 環境変数
 * @param[out] このコマンドを実行するかどうか
 */
#include<shell.h>
bool builtin_jobs(char *cmd,char *arg[],char *envp[])
{
  extern process prcs[PRCS];
  extern int prcn;
  if(strcmp(cmd,"jobs")==0)
    {
      int i;
      for(i=0;i<prcn;i++)
	printf("[%d] %s\n",prcs[i].pid,prcs[i].name);
      return true;
    }
  else
    return false;
}
/**
 * @brief シェル組み込みコマンドを実行する
 *
 * @param[in] cmd 入力されたコマンド
 * @param[in] arg 入力されたコマンドの引数
 * @param[in] envp 環境変数
 */
void builtin(char *cmd,char *arg[],char *envp[])
{
  static bool (*builtin_command[])(char*,char*[],char*[])=
    {
      builtin_exit,
      builtin_quit,
      builtin_jobs,
    }; /* 組み込みコマンドの一覧 */

  int i;
  int n=sizeof(builtin_command)/sizeof(builtin_command[0]);
  for(i=0;i<n;i++) /* 組み込みコマンドが呼ばれているか確認 */
    if(builtin_command[i](cmd,arg,envp))
      break;
}
