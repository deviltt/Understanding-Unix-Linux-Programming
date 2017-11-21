#include "stdio.h"
#include "termios.h"
#include "fcntl.h"
#include "string.h"

#define QUESTION "Do you want another transaction"
#define sleeptime 2
#define tries 3
#define BEEP putchar('\a')

int get_ok_char()
{
	int c;
	while((c = getchar()) != EOF && strchr("yYnN",c) == NULL);
	return c;
}

int get_response(char *question, int maxtries)
{
	int input;
	printf("%s (y/n)?", question);
	fflush(stdout);
	while(1){
		sleep(sleeptime);
		input = tolower(get_ok_char());
		if(input == 'y')
			return 0;
		if(input == 'n')
			return 1;
		if(maxtries-- == 0)
			return 2;
		BEEP;
		}
}

int tty_mode(int how)
{
	static struct termios original_mode;
	static int original_flags;
	if(how == 0){
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
	}
	else{
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flags);
	}
}

void set_crmode()
{
	struct termios ttystate;
	tcgetattr(0, &ttystate);		//获取标准输入tty的属性
	ttystate.c_lflag    &= ~ICANON;
	ttystate.c_lflag    &= ~ECHO;
	ttystate.c_cc[VMIN]  = 1;		//告诉程序一次可以读取多少字符
	tcsetattr(0, TCSANOW, &ttystate);
}

void set_nodelay_mode()
{
	int termflags;
	termflags = fcntl(0, F_GETFL);
	termflags |= O_NDELAY;
	fcntl(0, F_SETFL, termflags);
}

int main()
{
	int response;
	tty_mode(0);
	set_crmode();
	set_nodelay_mode();
	response = get_response(QUESTION, tries);
	tty_mode(1);
	printf("\n");
	return response;
}
