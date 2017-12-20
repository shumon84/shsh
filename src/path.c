/**
 * @file path.c
 *
 * Copyright(c) 2017 藤田朱門
 *
 * This software is released under the MIT License.
 * http;//opensource.org/licenses/MIT
 *
 * @brief コマンドサーチの実装
 * @author 藤田朱門
 * @date 2017/11/25
 */
#include<stdio.h>
#include<stdlib.h>

#include<path.h>
#include<builtin.h>

char *pth[PTHS];		/* コマンドサーチパス */
int pthnum;			/* パスの数 */

/**
 * @brief コマンドが存在するかPATHを検索
 *
 * @param[in] arg 検索するコマンド
 * @param[in,out] cmd コマンドのフルパス
 */
#include<sys/stat.h>
char* path_search(char arg[],char cmd[])
{
  int i;
  struct stat buf;
  for(i=0;i<pthnum;i++)
    {
      sprintf(cmd,"%s/%s",pth[i],arg);
      if(stat(cmd,&buf)==0)
	return cmd;
    }
  return NULL;
}

/**
 * @brief PATHを分割する
 *
 * 環境変数PATHを「:」を区切り時として分割する
 * 分割後のPATHはpthに格納される
 */
void path_set()
{
  int i;
  pth[pthnum++]=getenv("PATH");	/* 第一引数をセット */
  for(i=0;pth[0][i]!='\0';i++)
    {
      if(pth[0][i]==':')	/* 区切り字を見つけたとき */
  	{
  	  pth[0][i]='\0';
	  i++;
	  pth[pthnum++]=&pth[0][i]; /* 次のPATHに代入 */
  	}
    }
}
