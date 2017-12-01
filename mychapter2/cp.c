#include "stdio.h"
#include "fcntl.h"
#include "unistd.h"
#include "stdlib.h"

#define oops(s,x) {perror(s); exit(x);}
#define BUFFSIZE 4096

int main(int argc, char *argv[])
{
	int in_fd, out_fd, n_chars;
	char buf[BUFFSIZE];

	if(argc != 3){
		oops("usage: ./out source destination", 1);
	}
	if((in_fd = open(argv[1], O_RDONLY)) == -1)
		oops("open error", 1);	
	if((out_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC)) == -1)
		oops("open error", 1);

	while((n_chars = read(in_fd, buf, BUFFSIZE)) > 0)
		if(write(out_fd, buf, n_chars) != n_chars)
			oops("write error",1);

	close(in_fd);
	close(out_fd);	
}
