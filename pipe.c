#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#define oops(s,x) {perror(s); exit(x);}

int main(int argc, char *argv[])
{
	int pipefd[2],
	    newfd,
	    pid;

	if(argc != 3){
		fprintf(stderr, "usage:./xxx.out <cmd1 cmd2>\n");
		exit(1);
	}

	if(pipe(pipefd) == -1)
		oops("pipe error", 1);
	
	if((pid = fork()) == -1)
		oops("fork error", 2);

	if(pid == 0){
		close(pipefd[1]);		//关闭写
		if(dup2(pipefd[0], 0) == -1)	//将读重定向到标准输入
			oops("could not redirect stdout", 4);
		close(pipefd[0]);		//关闭读
		execlp(argv[1], argv[1], NULL);
		oops(argv[1], 5);
	}
	else{
		close(pipefd[1]);
		if(dup2(pipefd[0], 0) == -1)
			oops("could not redirect stdin", 3);
		close(pipefd[0]);
		execlp(argv[2], argv[2], NULL);
		oops(argv[2], 4);
	}
}
