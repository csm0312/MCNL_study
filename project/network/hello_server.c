#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[]="Hello World!";
	
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	// PF_INET = IPv4 인터넷 프로토콜 체계
	// SOCK_STREAM = TCP 소켓 <-> SOCK_DGRAM = UDP 소켓
	serv_sock=socket(PF_INET, SOCK_STREAM, 0); // 첫번째, 두번째 인자에서 결정되기 때문에 세번째 인자는 0이여도 상관없다
	if(serv_sock == -1)
		error_handling("socket() error");
	
	//memset = serv_addr의 주소를 시작으로 sizeof(serv_addr)만큼 0으로 초기화 하는 것이다.
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET; // 주소체계 저장 - AF_INET = IPv4 인터넷 프로토콜에 적용하는 주소체계
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); //32비트 IP주소 - INADDR_ANY = 현재 실행중인 컴퓨터의 IP를 소켓에 부여함
	serv_addr.sin_port=htons(atoi(argv[1])); // PORT번호 
	
	//server에 있는 소켓 주소를 할당하는 것이다. - 해당 ip와 port를 부여
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
		error_handling("bind() error"); 
	
	// 연결하는 것에 대한 가능한 상태로 변경하는 것이다. 소켓에 부여된 ip와 port로 다른 client가 입력시 연결을 허락한다.
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	clnt_addr_size=sizeof(clnt_addr);  
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");  
	
	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);	
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
