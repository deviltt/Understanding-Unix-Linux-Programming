#include "stdio.h"
#include "termios.h"

#define opps(s,x) {perror(s);exit(x);}

int main(int argc, char *argv[])
{
	struct termios info;
	
	if(argc == 1)
		exit(0);
	if(tcgetattr(0,&info) == -1)		//读取tty驱动程序的属性
		opps("tctetattr", 1);

	if(argv[1][0] == 'y')
		info.c_lflag |= ECHO;
	else
		info.c_lflag &= ~ECHO;
	
	if(tcsetattr(0, TCSANOW, &info) == -1)	//设置tty驱动的属性，立即更新驱动程序的设置
		opps("tcsetattr", 2);
}
