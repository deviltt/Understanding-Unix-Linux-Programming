#include "stdio.h"
#include "signal.h"

#define INPUTLEN 100

void inthandler(int signum)
{
	printf("Called with signal %d\n", signum);
	sleep(signum);
	printf("done handling signal %d\n", signum);
}

int main()
{
	struct sigaction newhandler;
	sigset_t blocked;
	char x[INPUTLEN];

	newhandler.sa_handler = inthandler;
	
	newhandler.sa_flags   = SA_RESTART ;// SA_RESETHAND | SA_RESTART;

	sigemptyset(&blocked);
	sigaddset(&blocked, SIGQUIT);
	
	newhandler.sa_mask = blocked;

	if(sigaction(SIGINT, &newhandler, NULL) == -1)
		perror("sigaction");
	else
 		while(1){
			fgets(x, INPUTLEN, stdin);
			printf("input: %s", x);
		}
}
