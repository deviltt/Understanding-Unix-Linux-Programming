#include "stdio.h"

void parent_code(int child_pid)
{
	int wait_rv;
	int exit_value, core_dump, signal_num;
	int child_status;
	
	wait_rv = wait(&child_status);	//wait的返回值是子进程的进程号，子进程的返回内容存放在child_status中
	printf("done waiting for %d, wait returned: %d\n", child_pid, wait_rv);
	exit_value = child_status >> 8;
	core_dump  = child_status &  0x80;
	signal_num = child_status &  0x7f;
	printf("status: exit = %d, core = %d, signal = %d\n", exit_value, core_dump, signal_num);
}

void child_code(int seconds)
{
	printf("child %d here, will sleep %d seconds\n", getpid(), seconds);
	sleep(seconds);
	printf("child done, about to exit\n");
	exit(17);
}

int main()
{
	int my_pid;

	printf("Before, mypid is %d\n", getpid());
	
	if((my_pid = fork()) == -1)
		perror("fork");
	else if(my_pid == 0)
		child_code(5);
	else
		parent_code(my_pid);
}
