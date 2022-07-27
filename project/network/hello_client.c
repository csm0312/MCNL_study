#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;
	
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	// PF_INET = IPv4 인터넷 프로토콜 체계
	// SOCK_STREAM = TCP 소켓 <-> SOCK_DGRAM = UDP 소켓
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");
	
	//memset = serv_addr의 주소를 시작으로 sizeof(serv_addr)만큼 0으로 초기화 하는 것이다.
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET; // 주소체계 저장 - AF_INET = IPv4 프로토콜에 적용하는 주소체계
	
	// 32 비트 IP주소 - server를 오픈한 PC IP주소를 입력 - string -> network byte로 변환 (빅 엔디안으로)
	// CPU가 리틀엔디안 식으로 작업을 처리하기 때문에 출력시는 리틀 엔디안 방식으로 출력이 된다.
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]); 
	 
	serv_addr.sin_port=htons(atoi(argv[2])); //PORT 번호 - htons(바이트 순서의 변환) - 대부분의 pc는 리틀 엔디안을 사용하기 때문
	
	// 해당 ip와 port를 이용하여 server와 연결하는 것이다.
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
		error_handling("connect() error!");
	
	str_len=read(sock, message, sizeof(message)-1);
	if(str_len==-1)
		error_handling("read() error!");
	
	printf("Message from server: %s \n", message);  
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
