#include "stdio.h"
#include "signal.h"

#define INPUTLEN 100

/*void inthandler(int signum)
{
	printf("Received signal %d .. waiting\n", signum);
	sleep(5);
	printf("Leaving inthandler\n");
}
*/
void handler(int signum)
{
	if(signum == SIGQUIT){
		printf("Received signal %d .. waiting\n", signum);
		sleep(2);
		printf("Leaving quithandler\n");
	}
	else if(signum == SIGINT){
		printf("Received signal %d .. waiting\n", signum);
		sleep(5);
		printf("Leaving inthandler\n");
	}
}

int main(int argc, char *argv[])
{
	char input[INPUTLEN];
	int nchars;

	signal(SIGINT , handler);
	signal(SIGQUIT, handler);
	do{
		printf("\nType a message\n");
		nchars = read(0, input, (INPUTLEN -1));
		if(nchars == -1)
			perror("read returned an error");
		else{
			input[nchars] = '\0';
			printf("You typed: %s", input);
		}
	}
	while(strncmp(input, "quit", 4) != 0);
}
