#include "stdio.h"
#include "curses.h"
#include "signal.h"
#include "sys/time.h"
#include "string.h"

#define MESSAGE "hello"
#define BLANK   "     " 

int row;
int col;
int dir;

void move_msg(int signum)
{
	signal(SIGALRM, move_msg);	//重置信号
	move(row, col);			//移动光标到指定位置
	addstr(BLANK);			//打印字符
	col += dir;			
	move(row, col);
	addstr(MESSAGE);
	refresh();			//刷新屏幕
	
	if(dir == -1 && col <= 0)
		dir = 1;
	else if(dir == 1 && col + strlen(MESSAGE) >= COLS)
		dir = -1;
}

int set_ticker(int n_msec)
{
	struct itimerval new_timeset;
	long n_sec, n_usec;
	
	n_sec = n_msec / 1000;
	n_usec = (n_msec % 1000) * 1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usec;

	
	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usec;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

int main()
{
	int delay;
	int ndelay;
	int c;

	initscr(); 	//初始化curses库和tty
	crmode();	
	noecho();	//用户输入不会回显出来
	clear();	//清屏
	
	row   = 10;	//设置行
	col   = 0;	//设置列
	dir   = 1;	//控制字符移动的方向
	delay = 200;	//时间参数，单位为ms
	
	move(row, col);		//移动光标到指定位置
	addstr(MESSAGE);	//在当前位置打印字符串
	signal(SIGALRM, move_msg);	//时钟到信号
	set_ticker(delay);	//设置初始时间间隔和重复时间间隔

	while(1){
		ndelay = 0;
		c = getch();
		if(c == 'Q') break;
		if(c == ' ') dir = -dir;
		if(c == 'a' && delay > 2) ndelay = delay/2;
		if(c == 'd') ndelay = delay * 2;
		if(ndelay > 0)
			set_ticker(delay = ndelay);
	}
	endwin();
	return 0;
}
