/* 每个进程默认使用文件描述符0，1，2作为标准输入，标准输出，标准出错 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define oops(s, x) {perror(s); exit(x);}

int main()
{
	int pid;

	if((pid = fork()) == EOF)
		oops("fork error", 1);

	if(pid == 0){
		close(1);
	//	fd = creat("userlist", 0644);	//最小分配原则
		if(open("userlist", O_WRONLY | O_APPEND | O_CREAT, 0644) != 1){	
			oops("open error", 1);
		}
		else{
			execlp("who", "who", NULL);
			perror("execlp");
		}
		exit(1);
	}
	else{
		wait(NULL);
		printf("done, results are in userlist\n");
	}
	return 0;
}
