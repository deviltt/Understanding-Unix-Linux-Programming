#include "stdio.h"
#include "signal.h"
#include "termios.h"
#include "string.h"

void set_mode(int how)
{
	static struct termios original_mode;
	if(how == 0)
		tcgetattr(0, &original_mode);
	else
		tcsetattr(0, TCSANOW, &original_mode);
}
void f()
{
	char u[BUFSIZ];
	printf("Interrupted! OK to quit (y/n)?");
	if(fgets(u, BUFSIZ, stdin) != NULL && (u[0] == 'y' || u[0] == 'Y'))
		set_mode(1);
		exit(0);
}

void set_noecho_mode()
{
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] == 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

int main()
{
	int i;
	set_mode(0);
	set_noecho_mode();
	signal(SIGINT, f);
	for(i = 0; i < 5; i++){
		printf("hello\n");
		sleep(1);
	}	
	set_mode(1);
}
