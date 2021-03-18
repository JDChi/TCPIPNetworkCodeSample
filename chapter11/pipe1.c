#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char const *argv[])
{
	int fds[2];
	char str[] = "Who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

    // 创建管道，fds数组中保存用于 I/O 的文件描述符
	pipe(fds);
	// 子进程将同时拥有通过 pipe(fds) 获取的2个文件描述符，复制的不是管道，而是用于管道I/O的文件描述符 
	pid = fork();
	if(pid == 0){
		// 子进程向管道传递字符串
		write(fds[1] , str , sizeof(str));
	}else{
		// 父进程从管道接收字符串
		read(fds[0] , buf , BUF_SIZE);
		puts(buf);
	}
	return 0;
}