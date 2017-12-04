#include <stdio.h>
#include <fcntl.h>

int main()
{
	char buf[100];
	int fd;

	fgets(buf, 100, stdin);	printf("%s", buf);
	fgets(buf, 100, stdin);	printf("%s", buf);
	fgets(buf, 100, stdin);	printf("%s", buf);

	close(0);
	if((fd = open("./stdinread.c", O_RDONLY)) != 0)
		printf("counld not open fd 0\n");
	
	fgets(buf, 100, stdin);	printf("%s", buf);
	fgets(buf, 100, stdin);	printf("%s", buf);
	fgets(buf, 100, stdin);	printf("%s", buf);
		
	return 0;		
}
