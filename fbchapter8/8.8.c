#include "stdio.h"

#define oops(s,x) {perror(s); exit(x);}
#define DELAY 2

void child_call(int delay)
{
	printf("child here, mypid is %d, about to sleep and exit\n", getpid());
	sleep(delay);
	exit(17);
}

void parent_call(int npid1, int npid2)
{
	int wait_rt;
	printf("waiting for child1 %d and child2 %d\n", npid1, npid2);
	
	wait_rt = wait(NULL);
	printf("wait returned %d\n", wait_rt);
	
	wait_rt = wait(NULL);
	printf("wait returned %d\n", wait_rt);
}

int main()
{
	int newpid1, newpid2;
	
	printf("parent's pid is %d\n", getpid());
	
	if((newpid1 = fork()) == -1)
		oops("newpid1", 1);
	if(newpid1 == 0)
		child_call(DELAY);

	if((newpid2 = fork()) == -1)
		oops("newpid2", 1);
	if(newpid2 == 0)
		child_call(DELAY);
	
	parent_call(newpid1, newpid2);
	
	return 0;
}
