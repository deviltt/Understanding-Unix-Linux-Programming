#include "stdio.h"
#include "curses.h"
#include "signal.h"
#include "fcntl.h"
#include "string.h"

#define MESSAGE "hello"
#define BLANK   "     "

int row   = 10;
int col   = 0;
int dir   = 1;
int delay = 200;
int done  = 0;

void on_input(int signum)
{
	int c = getch();
	
	if(c == 'Q' || c == EOF)
		done = 1;
	else if(c == ' ')
		dir = -dir;
}

void enable_kbd_signals()
{
	int fd_flags;
	
	fcntl(0, F_SETOWN, getpid());
	fd_flags = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, (fd_flags | O_ASYNC));
}

void on_alarm(int signum)
{
	signal(SIGALRM, on_alarm);
	mvaddstr(row, col, BLANK);
	col += dir;
	mvaddstr(row, col, MESSAGE);
	refresh();
	
	//处理越界	
	if(dir == -1 && col <= 0)
		dir = 1;
	else if(dir == 1 && col + strlen(MESSAGE) >= COLS)
		dir = -1;
	move(LINES - 1, COLS - 1);
	refresh();	//没有这一句则上一句使光标在右下角闪烁就没用
}

int main()
{
	initscr();
	crmode();
	noecho();
	clear();
	
	signal(SIGIO, on_input);	//SIGIO 读入击键并根据读入的击键采取相应的行动	
	enable_kbd_signals();
	signal(SIGALRM, on_alarm);
	set_ticker(delay);
	
//	move(row, col);
//	addstr(MESSAGE);	
	
	while(!done);
	//	pause();
	endwin();
}
