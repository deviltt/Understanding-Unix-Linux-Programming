#include "stdio.h"
#include "netinet/in.h"
#include "time.h"
#include "string.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netdb.h"
#include "stdlib.h"

#define HOSTLEN 256
#define PORTNUM 13000
#define oops(msg) {perror(msg); exit(1);}

int main(int argc, char *argv[])
{
	struct sockaddr_in saddr;
//	struct hostent       *hp;
//	char hostname[HOSTLEN];
	int sock_id, sock_fd;
	FILE *sock_fp;
	char *ctime();
	time_t thetime;
// 1.向内核申请一个socket
	if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		oops("socket");
	
	bzero((void *)&saddr, sizeof(saddr));

//	gethostname(hostname, HOSTLEN);
//	hp = gethostbyname(hostname);

//	bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
// 2.绑定地址到socket上，地址包括主机和端口
	if(bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
		oops("bind");
	
	if(listen(sock_id, 1) != 0)
		oops("listen");
	while(1){
		sock_fd = accept(sock_id, (struct sockaddr *)NULL, NULL);
		printf("Wow! got a call!\n");
		if(sock_fd == -1)
			oops("accept");
		sock_fp = fdopen(sock_fd, "w");
		if(sock_fp == NULL)
			oops("fdopen");
		thetime = time(NULL);
		
		fprintf(sock_fp, "the time here is...");
		fprintf(sock_fp, "%s", ctime(&thetime));
		fclose(sock_fp);
	}
}
