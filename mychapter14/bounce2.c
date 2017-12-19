#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXMSG 10
#define TUNIT 20000

struct propset{
	char *str;
	int row;
	int col;
	int delay;
	int dir;
};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int setup(int argc_num, char *argv_string[], struct propset props[])
{
	int num_msg = (argc_num > MAXMSG) ? MAXMSG : argc_num;
	int i;

	for(i = 0; i < num_msg; i++){
		props[i].str = argv_string[i];
		props[i].row = i;
		props[i].delay = 10; //+ (rand() % 15);
		props[i].dir = 1;
	}

	initscr();
	crmode();
	noecho();
	clear();
	mvprintw(LINES - 1, 0, "'Q' to quit, '0' ... '%d' to bounce", num_msg - 1);
	return num_msg;
}

void *print_msg(void *arg)
{
	struct propset *info = (void *)arg;
	int len = strlen(info->str) + 2;
	int col = 0;

	while(1){
		usleep(info->delay * TUNIT);

		pthread_mutex_lock(&lock);
		
		move(info->row, col);
		addch(' ');
		addstr(info->str);
		addch(' ');
		move(LINES - 1, COLS - 1);
		refresh();
		
		pthread_mutex_unlock(&lock);

		col += info->dir;

		if(col <= 0 && info->dir == -1)
			info->dir = 1;
		else if(col + len >= COLS && info->dir == 1)
			info->dir = -1;
	}
}

int main(int argc, char *argv[])
{
	pthread_t tids[MAXMSG];
	struct propset props[MAXMSG];
	int num_msg;
	int i, c;

	if(argc == 1){
		printf("usage: string1 ... string[MAXMSG]\n");
		exit(1);
	}
	
	num_msg = setup(argc - 1, argv + 1, props);

	for(i = 0; i < num_msg; i++){
		if(pthread_create(&tids[i], NULL, print_msg, &props[i])){
			printf("can't creat thread");
			endwin();
			exit(0);
		}
	}	

	while(1){
		c = getch();
		if(c == 'Q') break;
		if(c == ' '){
			for(i = 0; i < num_msg; i++)
				props[i].dir = -props[i].dir;
		}
		if(c >= '0' && c <= '9'){
			i = c - '0';
			if(i < num_msg)
				props[i].dir = -props[i].dir;
		}
	}

	pthread_mutex_lock(&lock);
	for(i = 0; i < num_msg; i++)
		pthread_cancel(tids[i]);
	endwin();	
	return 0;
}

