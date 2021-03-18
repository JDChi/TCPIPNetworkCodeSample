#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	pid_t pid = fork();

	if(pid == 0){
		puts("I am a child process");
	}else{
		printf("Child process ID %d \n", pid);
		sleep(30);
	}

	if(pid == 0){
		puts("End child process");
	}else{
		puts("End parent process");
	}
	return 0;
}