#include "stdio.h"
#include "signal.h"

void f(int signum)
{
	printf("OUCH! \n");
}

int main()
{
	int i;
	signal(SIGINT, f);   	//f代表中断处理函数
	for(i=0;i<5;i++){
		printf("hello\n");	
		sleep(1);
	}
	return 0;
}
