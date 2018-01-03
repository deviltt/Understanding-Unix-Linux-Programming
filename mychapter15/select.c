#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>

#define oops(s) {perror(s); exit(1);}

void showdata(char *fname, int fd)
{
	char buf[BUFSIZ];
	int n;

	printf("%s: ", fname);
	fflush(stdout);

	n = read(fd, buf, BUFSIZ);
	if(n == -1)
		oops("read error");
	if(write(1, buf, n) != n)
		oops("write error");

	write(1, "\n", 1);
}

int main(int argc, char *argv[])
{
	int fd1, fd2;
	fd_set rset;
	int maxfd;
	int retval;
	struct timeval timeout;

	if(argc != 4){
		fprintf(stderr, "usage: %s file file timeout", *argv);
		exit(1);
	}

	if((fd1 = open(argv[1], O_RDONLY)) == -1)
		oops(argv[1]);
	if((fd2 = open(argv[2], O_RDONLY)) == -1)
		oops(argv[2]);
	
	maxfd = 1 + (fd1 > fd2 ? fd1 : fd2);

	while(1){
		FD_ZERO(&rset);
		FD_SET(fd1, &rset);
		FD_SET(fd2, &rset);

		timeout.tv_sec = atoi(argv[3]);
		timeout.tv_usec = 0;

		retval = select(maxfd, &rset, NULL, NULL, &timeout);

		if(retval == -1)
			oops("select error");
		if(retval > 0){
			if(FD_ISSET(fd1, &rset))
				showdata(argv[1], fd1);
			if(FD_ISSET(fd2, &rset))
				showdata(argv[2], fd2);
		}
		else
			printf("no input after %d seconds\n", atoi(argv[3]));
	}

}
