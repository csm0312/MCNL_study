#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
void error_handling(char* message);

typedef struct {
	int* clnt_sock;
	char* message;
}recieve_Data;

// client socket에 저장된 번호로 보내서 보내는 메시지를 client로 보낸다.
void* send_function(void* clnt_sock){
	int* cs=(int*)clnt_sock;	
	while(1){
		char message[30];
		printf("\nserver -> : ");
		scanf("%s",message);
		write(*cs,message,sizeof(message));
	}
}

// client socket에서 보낸 메시지를 server가 읽어드리고 출력한다.
void* recieve_function(void* rcvdt){
	recieve_Data* data =(recieve_Data*)rcvdt;
	while(1){
		int str_len=read(*(data->clnt_sock),data->message,sizeof(char)*30);	
		if(str_len!=-1){	
			printf("\n <- client : %s\n",data->message);
		}		
	}
}

int main(int argc, char *argv[]){
	int serv_sock;
	int clnt_sock;
	char message[30];
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;	

	printf("read port....\n");
	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	
	printf("set server socket\n");
	// serv_socket 설정
	serv_sock=socket(PF_INET, SOCK_STREAM,0);
	if(serv_sock==-1){
		error_handling("socket() error");	
	}
	printf("set server addr...\n");	
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	printf("binding...\n");
	// socket에 ip와 port 부여
	if(bind(serv_sock,(struct sockaddr*) &serv_addr,sizeof(serv_addr))==-1){
		error_handling("bind() error");
	}
	
	if(listen(serv_sock,5)==-1){
		error_handling("listen() error");
	}	
	printf("waiting...\n");
	
	clnt_addr_size =sizeof(clnt_addr);
	clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
	if(clnt_sock==-1) error_handling("accept() error");
	printf("accept!\n");
	
	recieve_Data rcvdt;
	rcvdt.clnt_sock=&clnt_sock;
	rcvdt.message=message;

	pthread_t p_thread[2];	
	int t;
	int status;
	for(t=0;t<2;t++){
		if(t==0) pthread_create(&p_thread[t],NULL,send_function,(void*)&clnt_sock);
		else if(t==1)pthread_create(&p_thread[t],NULL,recieve_function,(void*)&rcvdt);
	}
	
	pthread_join(p_thread[0],(void **)&status);
	pthread_join(p_thread[1],(void **)&status);

	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char* message){
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);
}