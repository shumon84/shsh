#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<err.h>

#define BUFS (1024)
#define ARGS (1024)

int flg_bg; /* bgプロセスのフラグ */

/**
 * @brief 受け取った文字列を走査して引数に分割
 *
 * 引数の分割時にbgプロセスのフラグ処理も同時に行う
 *
 * @param[in,out] arg 分割された引数の配列
 * @param[in,out] str 分割前の文字列
 */
void argdiv(char *arg[],char *str)
{
  int i,j;

  for(i=0;str[i]!='\0';i++)
    {
      if(str[i]==' ') /* 空白を見つけたとき */
	{
	  str[i]='\0'; /* 文字列を分割 */
	  while(str[++i]==' '); /* 連続する空白を読み飛ばす */
	  if(str[i]=='&')
	    flg_bg=1;
	  else
	    arg[++j]=&str[i]; /* 次の要素に代入 */
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
  char str[BUFS];
  /* プロンプトを表示して入力を受け取る */
  printf("$ ");
  fgets(str,BUFS,stdin);
  str[strlen(str)-1]='\0'; /* 改行を消去 */

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
 * @brief 子プロセスでの挙動
 *
 * @param[in] cmd コマンド
 * @param[in] arg 引数配列
 * @param[in] envp 環境変数
 */
void child(char *cmd,char *arg[],char *envp[])
{
  execve(cmd,arg,envp); /* コマンドを実行 */
  exit(0);
}

/**
 * @brief 親プロセスでの挙動
 *
 */
void parent()
{
  int status;
  if(flg_bg==0)
    wait(&status);
}

int main(int argc,char *argv[],char *envp[])
{
  pid_t pid;
  while(1)
    {
      char *arg[ARGS]={};

      wait_input(arg);
      exit_shell(arg[0],arg,envp);

      pid=fork();
      switch(pid)
	{
	case -1:perror("folk"); continue; /* folkに失敗した場合 */
	case 0:child(arg[0],arg,envp); break; /* 子プロセス */
	default:parent(); break; /* 親プロセス */
	}
    }

  return 0;
}
