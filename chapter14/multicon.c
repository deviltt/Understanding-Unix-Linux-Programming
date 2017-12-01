#include "stdio.h"
#include "pthread.h"
#include "curses.h"
#include "stdlib.h"

#define MAXMSG 10
#define TUNIT  20000

struct msg{
	char *str;
	int row;
	int delay;
	int dir;
};

pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

int setup(int strnum, char *strings[], struct msg msgs[])	//设置每一个字符的初始行，延时时间，方向，并显示
{
	int i;
	int num_msg = (strnum > MAXMSG ? MAXMSG : strnum);
	for(i = 0; i < strnum; i++){
		msgs[i].str = strings[i];
		msgs[i].row = i;
		msgs[i].delay = 1 + (rand() % 10);
		msgs[i].dir = ((rand() % 2) ? 1 : -1);
	}
	initscr();
	crmode();
	noecho();
	clear();
	mvprintw(LINES - 1, 0, "'Q' to quit, '0'...'%d' to bounce", num_msg - 1);
	return num_msg;
}

void *movestr(void *arg)
{
	struct msg *info = arg;
	int len = strlen(info->str) + 2;
	int col = rand() % (COLS - len - 3);
	
	while(1){
		usleep(info->delay * TUNIT);
	
		pthread_mutex_lock(&mx);	
		move(info->row, col);
		addch(' ');
		addstr(info->str);
		addch(' ');
		move(LINES-1, COLS-1);
		refresh();
		pthread_mutex_unlock(&mx);
	
		col += info->dir;
		if(col <= 0 && info->dir == -1)
			info->dir = 1;
		else if(col + len >= COLS && info->dir == 1)
			info->dir = -1;
	}
}

int main(int argc, char *argv[])
{
	int msg_num;
	int c;
	pthread_t 	thrds[MAXMSG];
	struct msg  	msgs[MAXMSG];
	int i;

	if(argc == 1){
		printf("usage: string ...\n");
		exit(1);
	}
	msg_num = setup(argc-1, argv+1, msgs);

	for(i = 0; i < msg_num; i++){
		if(pthread_create(&thrds[i], NULL, movestr, &msgs[i])){
			fprintf(stderr, "error creating thread");
			endwin();
			exit(0);
		}
	}

	while(1){
		c = getch();
		if(c == 'Q') break;
		if(c == ' '){
			for(i = 0; i < msg_num; i++)
				msgs[i].dir = -msgs[i].dir;
		}
		if(c >= '0' && c <= '9'){
			i = c - '0';
			if(i < msg_num)
				msgs[i].dir = -msgs[i].dir;
		}
	}
	pthread_mutex_lock(&mx);
	for(i = 0; i < msg_num; i++)
		pthread_cancel(thrds[i]);
	endwin();
	return 0;
}
