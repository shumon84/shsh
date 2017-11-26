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

#include<builtin.h>

/**
 * @brief シェルを終了させる組み込みコマンド
 *
 * @param[in] cmd 入力されたコマンド
 * @param[in] arg 入力されたコマンドの引数
 * @param[in] envp 環境変数
 */
void builtin_exit(char *cmd,char *arg[],char *envp[])
{
  if(strcmp(cmd,"exit")==0)
    exit(0);      
}

/**
 * @brief シェルを終了させる組み込みコマンド
 *
 * @param[in] cmd 入力されたコマンド
 * @param[in] arg 入力されたコマンドの引数
 * @param[in] envp 環境変数
 */
void builtin_quit(char *cmd,char *arg[],char *envp[])
{
  if(strcmp(cmd,"quit")==0)
    exit(0);
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
  static void (*builtin_command[])(char*,char*[],char*[])=
    {
      builtin_exit,
      builtin_quit,
    };				/* 組み込みコマンドの一覧を作る */

  int i;
  int n=sizeof(builtin_command)/sizeof(void *(char*,char*[],char*[]));
  for(i=0;i<n;i++) /*  */
    builtin_command[i](cmd,arg,envp);
}
