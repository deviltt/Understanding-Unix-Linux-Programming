#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"

#define CLOSE_DUP
#define USE_DUP2

int main()
{
	int fd, newfd;
	char line[100];

	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	
	fd = open("./stdinredir.c", O_RDONLY);
#ifdef CLOSE_DUP
	close(0);
	newfd = dup(fd);
#else
	newfd = dup2(fd, 0);	//close 0, dup fd to 0
#endif
	if(newfd != 0){
		fprintf(stderr, "could not duplicate fd to 0");
		exit(1);
	}
	close(fd);
	
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
	fgets(line, 100, stdin); printf("%s", line);
}
