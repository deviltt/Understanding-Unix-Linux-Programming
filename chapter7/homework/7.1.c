#include "stdio.h"
#include "signal.h"

void wakeup(int signum)
{
	printf("Alarm received from kernel\n");
}

int main()
{
	printf("About to sleep for 4 seconds\n");
	signal(SIGALRM, wakeup);
	signal(SIGINT, SIG_IGN);
	alarm(4);
	pause();
	printf("Morning so soon?\n");
}
