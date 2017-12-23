#include <stdio.h>
#include <fcntl.h>

int main()
{
	char buf[100];
	int  fd;

	fgets(buf, 100, stdin);
	printf("%s", buf);
	
	close(0);
	if((fd = open("./redefine_std.c", O_RDONLY)) == EOF)
		perror("open error");

	fgets(buf, 100, stdin);
	printf("%s", buf);
	
	return 0;
}
