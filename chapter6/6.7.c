#include "stdio.h"
#include "signal.h"

int quitnum;

void f()
{
	static int hits;
	int i;
	hits++;
	printf("OUCH");
	i = hits;
	while(i--)
		printf("!");
	printf("\n");
	if(hits == quitnum)
		exit(0);
}

int main(int argc, char *argv[])
{
	if(argc > 1 && atoi(argv[1]) > 0 )
		quitnum = atoi(argv[1]);  	//将字符串转换成整数
	signal(SIGINT, f);
	while(1){
		sleep(1);
		printf("hello\n");
	}
}
