#include "stdio.h"
#include "signal.h"
#include "termios.h"

int main()
{
	struct termios ttystate;
	int i;
//	pid_t pid;

//	pid = getpid();
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ISIG;
	tcsetattr(0, TCSANOW, &ttystate);
//	kill(pid, SIGINT);
	printf("you can't kill me\n");
	for(i = 0; i < 6; i++){
		sleep(1);
		printf("haha!\n");
		if(i == 3){
			printf("laugh your sister, i will kill you!\n");
			sleep(2);
			kill(getpid(), SIGINT);
		}
	}
}
