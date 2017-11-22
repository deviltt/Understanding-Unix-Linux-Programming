#include "stdio.h"
#include "fcntl.h"
#include "termios.h"
#include "signal.h"
#include "string.h"

#define ASK "will you take money?"
#define TRIES 3
#define BEEP putchar('\a')

void set_mode(int how)
{
	static struct termios original_mode;
	static int original_flag;
	
	if(how == 0){
		tcgetattr(0, &original_mode);
		original_flag = fcntl(0, F_GETFL);
	}
	else{
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flag);
	}
}

void set_noecho_mode()
{
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	ttystate.c_lflag &= ~ICANON;
	ttystate.c_lflag &= ~ECHO;
	ttystate.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &ttystate);
}

void set_fflag_mode()
{
	int fflags;
	fflags = fcntl(0, F_GETFL);
	fflags |= O_NDELAY;
	fcntl(0, F_SETFL, fflags);
}

int get_ok_char()
{
	int c;
	while((c = getchar()) != EOF && strchr("YyNn", c) == NULL);
	return c;
}

int get_response(char *ask, int tries)
{
	int input;
	printf("%s (y/n)", ask);
	fflush(stdout);
	while(1){
		sleep(2);
		input = tolower(get_ok_char());
		if(input == 'y')
			return 0;
		if(input == 'n')
			return 1;
		if(tries-- == 0)
			return 2;
		BEEP;
	}
}

void getout(int sig)
{
	set_mode(1);
	printf("\n");
	exit(2);
}

int main()
{
	int response;

  //1. 保留当前驱动设置状态和文件标志状态
	set_mode(0);

	signal(SIGINT, getout);
	signal(SIGQUIT, getout);

  //2. 设置驱动设置状态
	set_noecho_mode();
  //3. 设置文件标志状态
	set_fflag_mode();
  //4. 回显文字
	response = get_response(ASK, TRIES);
  //5. 回复1中保留的状态
	set_mode(1);
	printf("\n");
	return response;
}
