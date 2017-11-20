#include "stdio.h"
#include "termios.h"

int main()
{
	struct termios info;
	int rv;

	rv = tcgetattr(0, &info);		//0是标准输入的文件描述符，该函数的作用是读取标准输入的属性
	if(rv == -1){
		perror("tcgetattr");
		exit(1);
	}
	if(info.c_lflag & ECHO)
		printf("echo is on, since its bit is 1\n");
	else
		printf("echo is off, since its bit is 0\n");
	
	return 0;
}
