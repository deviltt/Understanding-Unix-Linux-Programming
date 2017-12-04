#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
	int fd, pid;

	if((pid = fork()) == EOF)
		exit(1);
	
	if(pid == 0){
		close(0);
		fd = open("./sort.c", O_RDONLY);
		execlp("sort", "sort", NULL);
		perror("execlp");
		exit(1);
	}
	else{
		wait(NULL);
		printf("done\n");
	}
	return 0;
}
