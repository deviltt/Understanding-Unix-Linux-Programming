#include "stdio.h"

void child_code(int a)
{
	printf("child %d here, will sleep %d seconds\n", getpid(), a);
	sleep(a);
	exit(1);
}

void parent_code(int a)
{
	int wait_rv;
	wait_rv = wait(NULL);
	printf("done wating for %d. Wait returned: %d\n", a, wait_rv);
}

int main()
{
	int my_pid;

	printf("Before, mypid is %d\n", getpid());

	if((my_pid = fork()) == -1)
		perror("fork");
	else if(my_pid == 0)
		child_code(2);
	else
		parent_code(my_pid);
}
