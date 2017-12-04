#include <stdio.h>
#include <fcntl.h>

int main()
{
	int fd, newfd;
	char buf[100];

	fgets(buf, 100, stdin);	printf("%s", buf);
	fgets(buf, 100, stdin);	printf("%s", buf);	
	fgets(buf, 100, stdin);	printf("%s", buf);

	if((fd = open("./stdinread.c", O_RDONLY)) == EOF){
		printf("open error");
		exit(0);
	}	
	
	close(0);
	if((newfd = dup(fd)) != 0){
		fprintf(stderr, "dup error");
		exit(1);
	}
	close(fd);
	fgets(buf, 100, stdin);	printf("%s", buf);
	fgets(buf, 100, stdin);	printf("%s", buf);
	fgets(buf, 100, stdin);	printf("%s", buf);
	
	exit(0);
}
