#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void * handle_clnt(void * arg);
void send_msg(char * msg , int len);
void error_handling(char *message);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

int main(int argc, char const *argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_adr_sz;
	pthread_t t_id;

	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutex , NULL);
	serv_sock = socket(PF_INET , SOCK_STREAM , 0);
	memset(&serv_addr , 0 , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock , (struct sockaddr*) &serv_addr , sizeof(serv_addr)) == -1){
		error_handling("bind() error");
	}

	if(listen(serv_sock , 5) == -1){
		error_handling("listen() error");
	}

	while(1)
	{
		clnt_adr_sz = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock , (struct sockaddr*)&clnt_addr , &clnt_adr_sz);

		pthread_mutex_lock(&mutex);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutex);

		pthread_create(&t_id , NULL , handle_clnt , (void*)&clnt_sock);
		pthread_detach(t_id);
		printf("Connected client IP : %s \n", inet_ntoa(clnt_addr.sin_addr));
	}
	close(serv_sock);

	return 0;
}

void * handle_clnt(void * arg)
{
	int clnt_sock=*((int*)arg);
	int str_len = 0 , i;
	char msg[BUF_SIZE];

	while((str_len=read(clnt_sock , msg , sizeof(msg))) != 0)
		send_msg(msg , str_len);

	pthread_mutex_lock(&mutex);
	for (int i = 0; i < clnt_cnt; i++)
	{
		if(clnt_sock == clnt_socks[i])
		{
			while(i++ < clnt_cnt-1)
				clnt_socks[i] = clnt_socks[i + 1];
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void send_msg(char * msg , int len)
{
	int i;
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < clnt_cnt; i++)
	{
		write(clnt_socks[i] , msg , len);
	}
	pthread_mutex_unlock(&mutex);

}

void error_handling(char *message){
	fputs(message , stderr);
	fputc('\n' , stderr);
	exit(1);
}
