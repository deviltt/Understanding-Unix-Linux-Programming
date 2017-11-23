#include "stdio.h"
#include "signal.h"

void wakeup(int signum)
{
	printf("Alarm received from kernel\n");
}

int main()
{
	printf("about to sleep 4 seconds\n");
	signal(SIGALRM, wakeup);
	alarm(4);	//时钟一到，就会发送SIGLRM信号给进程
	printf("aaa\n");//在等待的时候可以做其他事情
	pause();	//pause函数的作用是挂起一个进程，直到有一个信号被处理
	printf("Morning so soon?\n");
}
