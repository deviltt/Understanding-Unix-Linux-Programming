#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define oops(s) {perror(s); exit(1);}

void showdata(char *fname, int fd)
{
	char buf[BUFSIZ];
	int n;

	printf("%s ", fname);
	fflush(stdout);
	n = read(fd, buf, BUFSIZ);
	if(n == EOF)
		oops("read error");
	if(write(1, buf, n) != n)
		oops("write error");
	write(1, "\n", 1);
}

int main(int argc, char *argv[])
{
	int fd1, fd2;
	struct timeval timeout;
	fd_set readfds;
	int maxfd;
	int retval;

	if(argc != 4)
		fprintf(stderr, "usage: %s file1 file2 timeval", *argv);

	if((fd1 = open(argv[1], O_RDONLY)) == EOF)
		oops("open1 error");
	if((fd2 = open(argv[2], O_RDONLY)) == EOF)
		oops("open2 error");

	maxfd = 1 + (fd1 > fd2 ? fd1 : fd2);

	while(1){
		FD_ZERO(&readfds);
		FD_SET(fd1, &readfds);
		FD_SET(fd2, &readfds);

		timeout.tv_sec = atoi(argv[3]);
		timeout.tv_usec = 0;

		retval = select(maxfd, &readfds, NULL, NULL, &timeout);
		
		if(retval == -1)
			oops("select");
		if(retval > 0){
			if(FD_ISSET(fd1, &readfds))
				showdata(argv[1], fd1);
			if(FD_ISSET(fd2, &readfds))
				showdata(argv[2], fd2);
		}
		else
			printf("no input after %d seconds\n", atoi(argv[3]));
	}
}
