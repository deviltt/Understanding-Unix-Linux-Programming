#include "stdio.h"
#include "fcntl.h"
#include "signal.h"
#include "termios.h"
#include "string.h" 

#define ASK "Do you want another transaction"
#define TRIES 3
#define SLEEPTIME 2
#define BEEP putchar('\a')

void set_noecho_cr()
{
	struct termios ttystate;
	
	tcgetattr(0, &ttystate);     		//获取标准输入的配置信息
	ttystate.c_lflag &= ~ICANON;		//参数设置为无缓冲输入	
	ttystate.c_lflag &= ~ECHO;		//参数设置为无回显信息
	ttystate.c_cc[VMIN] = 1;		//最小输入一个字符
	tcsetattr(0, TCSANOW, &ttystate);	//设置标准输入的配置信息
}

void set_nodelay()
{
	int termflags;
	termflags = fcntl(0, F_GETFL);		//获取标准输入文件标志
	termflags |= O_NDELAY;			//将文件标志设置为无阻塞模式
	fcntl(0, F_SETFL, termflags);		//设置标准输入文件标志
}

void tty_mode(int how)
{
	static struct termios original_mode;
	static int original_flags;
	static int stored = 0;
	
	if(how == 0){
		tcgetattr(0, &original_mode);
		original_flags = fcntl(0, F_GETFL);
		stored = 1;
	}	
	else if(stored){
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flags);
	}
}
/*
void ctrl_c_handler(int signum)			//目的是为了检测是否捕捉到中断信息
{
	tty_mode(signum);
	exit(1);
}
*/
int get_ok_char()
{
	int c;
	while((c = getchar()) != EOF && strchr("yYnN", c) == NULL);
	return c;
}

int get_response(char *question, int maxtries)
{
	int input;
	printf("%s(y/n)?", question);
	fflush(stdout);
	while(1){
		sleep(SLEEPTIME);
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

int main()
{
	int response;

	tty_mode(0);
	set_noecho_cr();
	set_nodelay();
//	signal(SIGINT, ctrl_c_handler);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	response = get_response(ASK, TRIES);
	tty_mode(1);
	printf("\n");
	return response;
}
