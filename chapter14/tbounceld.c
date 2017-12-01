#include "stdio.h"
#include "pthread.h"
#include "curses.h"
#include "stdlib.h"
#include "unistd.h"

#define MESSAGE " hello "

int row;
int col;
int dir;
int delay;

void *moving_msg(void *msg)
{
	while(1){
		usleep(delay * 1000);
		move(row, col);
		addstr(msg);
		refresh();

		col += dir;
	
		if(col <= 0 && col == -1)
			dir = 1;
		else if(col + (sizeof(msg)-1) >= COLS && dir == 1)
			dir = -1;
	}
}

int main()
{
	int ndelay;
	int c;
	pthread_t msg_thread;
	
	initscr();
	crmode();
	noecho();
	clear();

	row   = 10;
	col   = 0;
	dir   = 1;
	delay = 200;
	 
	if( pthread_create(&msg_thread,NULL,moving_msg,MESSAGE) ){
		fprintf(stderr, "creating pthread error");
		endwin();
		exit(0);
	}
	
	while(1){
		ndelay = 0;
		c = getch();
		if(c == 'Q') break;
		if(c == ' ') dir = -dir;
		if(c == 'f' && delay > 2) ndelay = delay / 2;
		if(c == 's') ndelay = delay * 2;
		if(ndelay > 0)
			delay = ndelay;
	}
	pthread_cancel(msg_thread);
	endwin();
}
