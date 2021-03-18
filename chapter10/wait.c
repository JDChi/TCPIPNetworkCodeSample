#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
	int status;
	pid_t pid = fork();

	if(pid == 0){
		return 3;
	}else{
		printf("Child process ID %d \n", pid);
		pid=fork();
		if(pid == 0){
			exit(7);
		}else{
			printf("Child process ID %d \n", pid);
			wait(&status);
			if(WIFEXITED(status)){
				printf("Child send one : %d \n", WEXITSTATUS(status));
			}

			wait(&status);

			// 因为创建了2个进程，因此再次调用wait函数和宏
			if(WIFEXITED(status)){
				printf("Child send one : %d \n", WEXITSTATUS(status));
			}
			sleep(30);
		}
	}

	if(pid == 0){
		puts("End child process");
	}else{
		puts("End parent process");
	}
	return 0;
}