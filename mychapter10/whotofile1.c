#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
	int pid;
	int fd;

	if((pid = fork()) == EOF){
		perror("fork error");
		exit(1);
	}

	if(pid == 0){
		close(1);
		open("usrlist", O_RDWR | O_APPEND | O_CREAT, 0777);
		execlp("who", "who", NULL);
		perror("execlp");
		exit(1);
	}
	else{
		wait(NULL);
		printf("done who to file\n");
	}
}
