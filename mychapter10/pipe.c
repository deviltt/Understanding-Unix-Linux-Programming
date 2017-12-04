#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define oops(s, x) {perror(s); exit(x);}

int main(int argc, char *argv[])
{
	int pipefd[2], fd, pid;

	if(argc < 3){
		fprintf(stderr, "usage: ./.out cmd1 cmd2\n");
		exit(1);
	}
	
	if(pipe(pipefd) == EOF)
		oops("pipe error", 1);
	
	if((pid = fork()) == EOF){
		oops("fork error", 2);
	}
	else if(pid == 0){		//把执行的命令行命令内容写进管道
		close(pipefd[0]);
		if(dup2(pipefd[1], 1) == EOF)
			oops("dup error", 3);
		close(pipefd[1]);
		execlp(argv[1], argv[1], NULL);
		oops("execlp", 4);
	}
	else{
		close(pipefd[1]);	//从管道中读入shell命令所需的内容
		if(dup2(pipefd[0], 0) == EOF)
			oops("dup error", 4);
		close(pipefd[0]);
		execlp(argv[2], argv[2], NULL);
		oops("execlp", 5);
	}
	return 0;
}
