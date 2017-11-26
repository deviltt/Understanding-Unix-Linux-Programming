#include "stdio.h"
#include "signal.h"
#include "string.h"
#include "stdlib.h"

#define MAXARGS 20
#define ARGLEN  100

char * makestring(char *buf)
{
	char *cp;
	buf[strlen(buf) - 1] = '\0';	//strlen计算不包括含终止NULL字节的字符串长度，sizeof包括
	cp = (char *)malloc(strlen(buf) + 1);
	if(cp == NULL){
		fprintf(stderr, "no memory\n");
		exit(1);
	}
	strcpy(cp, buf);
	return cp;
}

void execute(char *arglist[])
{
	int pid,exitstatus;
	
	pid = fork();
	switch(pid){
		case -1:
			perror("fork failed");	
			exit(1);
		case  0:
			execvp(arglist[0], arglist);	//通过调用exec函数族在当前进程中执行一个新的程序
			perror("execvp failed");
			exit(1);	
		default:
			while(wait(&exitstatus) != pid);	//当接受到子进程的退出信息时就会继续执行被挂起的进程
			printf("child exited with status %d, %d\n", exitstatus >> 8, exitstatus & 0377);
	}
}

int main()
{
	char *arglist[MAXARGS];
	char argbuf[ARGLEN];
	int numargs;

	numargs = 0;
	while(numargs < MAXARGS){
		printf("Arg[%d]?", numargs);
		if(fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
			arglist[numargs++] = makestring(argbuf); //在每个字符串后面添加空字符即NULL
		else{
			if(numargs > 0){
				arglist[numargs] = NULL;
				execute(arglist);
				numargs = 0;
			}
		}
	}
	return 0;
}
