#include <stdio.h>
#include <fcntl.h>

int main()
{
	int fd, newfd;
	char buf[100];

	fgets(buf, 100, stdin);
	printf("%s", buf);

	if((fd = open("./redefine_std.c", O_RDONLY)) == EOF)
		perror("open error");

	newfd = dup2(fd, 0);
	close(fd);

	fgets(buf, 100, stdin);
	printf("%s", buf);
	
	return 0;
}
