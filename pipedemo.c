#include "stdio.h"
#include "unistd.h"
#include "string.h"

int main()
{
	int i, len, apipe[2];
	char buf[BUFSIZ];

	if(pipe(apipe) == -1){
		perror("could not make pipe");
		exit(1);
	}
	printf("get a pipe, it's file descriptors:{ %d  %d }\n", apipe[0], apipe[1]);

	while(fgets(buf, BUFSIZ, stdin)){
		len = strlen(buf);
		if(write(apipe[1], buf, len) != len){
			perror("writing to pipe");
			break;
		}
		for(i = 0; i < len; i++)
			buf[i] = 'X';
		len = read(apipe[0], buf, BUFSIZ);
		if(len == -1){
			perror("reading from pipe");
			break;
		}
		if(write(1, buf, len) != len){
			perror("writing to stdout");
			break;
		}
	}
}
