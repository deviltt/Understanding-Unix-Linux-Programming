#include "stdio.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netdb.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"

#define oops(s) {perror(s); exit(1);}

int main(int argc, char *argv[])
{
	struct sockaddr_in servadd;
	struct hostent *hp;
	int sock_id, sock_fd;
	char message[BUFSIZ];
	int messlen;

	//step 1:get a socket
	if((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		oops("socket");
	
	bzero(&servadd, sizeof(servadd));
//	hp = gethostbyname(argv[1]);
//	if(hp == NULL)
//		oops(argv[1]);
//	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	servadd.sin_port = htons(13000);
	servadd.sin_family = AF_INET;
	if(inet_pton(AF_INET, argv[1], &servadd.sin_addr) <=0)
		oops("inet_pton error");	

	if(connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0)
		oops("connect");

	messlen = read(sock_id, message, BUFSIZ);
	if(messlen == -1)
		oops("read");
	if(write(1, message, messlen) != messlen)
		oops("write");
	close(sock_id);
}
