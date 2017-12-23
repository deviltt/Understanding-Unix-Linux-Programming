#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
	int pid, fd;

	if((pid = fork()) == EOF){
		perror("fork error");
		exit(1);
	}

	if(pid == 0){
		close(0);
		open("./sort.c", O_RDONLY);
		if(execlp("sort", "sort", NULL) == EOF)
			perror("execlp error");
	}
	else{
		wait(NULL);
		printf("file to sort done\n");
	}
	return 0;
}
