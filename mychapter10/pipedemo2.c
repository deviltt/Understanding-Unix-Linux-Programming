#include <stdio.h>
#include <stdlib.h>

#define oops(s,x) {perror(s); exit(x);}
#define PMESG "hello, son\n"
#define SMESG "hello, father\n"

int main()
{
	int pipefd[2], fd, len, read_len;
	char buf[BUFSIZ];

	if(pipe(pipefd) == EOF)
		oops("pipe error", 1);

	if((fd = fork()) == EOF){
		oops("fork error", 2);
	}
	else if(fd == 0){
		len = strlen(SMESG);
		while(1){
		if(write(pipefd[1], SMESG, len) != len)
			oops("write error", 3);
	
/*		if((read_len = read(pipefd[0], buf, BUFSIZ)) <= 0)
			oops("read error", 4);
		if(write(1, buf, read_len) != read_len)
			oops("write error", 5);*/
		sleep(4);
		}
	}
	else{
		len = strlen(PMESG);
		while(1){
		if(write(pipefd[1], PMESG, len) != len)
			oops("write error", 6);
		sleep(1);
		if((read_len = read(pipefd[0], buf, BUFSIZ)) <= 0)
			oops("read error", 7);
		if(write(1, buf, read_len) != read_len)
			oops("write error", 8);}
	}
	return 0;
}
