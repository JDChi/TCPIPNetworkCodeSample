#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/**
 * 信号处理器
**/
void timeout(int sig){
	if(sig == SIGALRM){
		puts("Time out!");
		// 每隔2秒重复产生 SIGALRM 信号
		alarm(2);
	}
}

/**
 * 信号处理器
**/
void keycontrol(int sig){
	if(sig == SIGINT){
		puts("CTRL+C pressed");
	}
}

int main(int argc, char const *argv[])
{
	int i;
	signal(SIGALRM , timeout);
	signal(SIGINT , keycontrol);

	// 预约2秒后发生 SIGALRM 信号
	alarm(2);

	for(int i = 0 ; i < 3 ; i++){
		puts("wait...");
		sleep(100);
	}
	return 0;
}