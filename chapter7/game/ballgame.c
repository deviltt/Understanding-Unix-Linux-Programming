#include "stdio.h"
#include "signal.h"
#include "sys/time.h"
#include "curses.h"

#define TOP_ROW 5
#define BOT_ROW 20
#define LEFT_COL 10
#define RIGHT_COL 60
#define BALL 'o'
#define Y_INIT 10
#define X_INIT 10
#define BLANK ' '
#define X_TTM 2
#define Y_TTM 2

struct ppball{
	int y_pos, x_pos,
	    y_ttm, x_ttm,
	    y_ttg, x_ttg,
	    y_dir, x_dir;
	char symbol;
};

struct ppball the_ball;

int set_ticker(int n_msec)
{
	struct itimerval new_timeset;
	long n_sec, n_usec;

	n_sec = n_msec / 1000;
	n_usec = (n_msec % 1000) * 1000L;

	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usec;
	
	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usec;
	
	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

int bounce_or_lose(struct ppball *bp)
{
	int return_val = 0;

	if(bp->y_pos == TOP_ROW){
		bp->y_dir = 1;
		return_val = 1;
	}else if(bp->y_pos == BOT_ROW){
		bp->y_dir = -1;
		return_val = 1;
	}

	if(bp->x_pos == LEFT_COL){
		bp->x_dir = 1;
		return_val = 1;
	}else if(bp->x_pos == RIGHT_COL){
		bp->x_dir = -1;
		return_val = 1;
	}
	return return_val;
}

void ball_move(int signum)
{
	int y_cur, x_cur, moved;

	signal(SIGALRM, SIG_IGN);
	y_cur = the_ball.y_pos;
	x_cur = the_ball.x_pos;
	moved = 0;
	if(the_ball.y_ttm > 0 && the_ball.y_ttg-- == 1){	//y_ttg为2，则重复时间为40ms
		the_ball.y_pos += the_ball.y_dir;
		the_ball.y_ttg = the_ball.y_ttm;
		moved = 1;
	}

	if(the_ball.x_ttm > 0 && the_ball.x_ttg-- == 1){
		the_ball.x_pos += the_ball.x_dir;
		the_ball.x_ttg = the_ball.x_ttm;
		moved = 1;
	}
	
	if(moved){
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
		bounce_or_lose(&the_ball);
		move(LINES - 1, COLS -1);	//目的是将光标始终放在右下角
		refresh();
	}
	signal(SIGALRM, ball_move);
}

void set_up()
{
	the_ball.y_pos = Y_INIT;
	the_ball.x_pos = X_INIT;

	the_ball.y_ttg = the_ball.y_ttm = Y_TTM;
	the_ball.x_ttg = the_ball.x_ttm = X_TTM;

	the_ball.y_dir = 1;
	the_ball.x_dir = 1;
	the_ball.symbol = BALL;

	initscr();
	noecho();
	crmode();

	signal(SIGINT, SIG_IGN);
	mvaddch(the_ball.y_pos, the_ball.x_pos, the_ball.symbol);
	refresh();

	signal(SIGALRM, ball_move);
	set_ticker(1000 / 500);   	//初始时间间隔为0，重复时间间隔为20ms
}

void main()
{
	int c;

	set_up();
	while((c = getchar()) != 'Q'){
		if(c == 'f')	  the_ball.x_ttm--;
		else if(c == 's') the_ball.x_ttm++;
		else if(c == 'F') the_ball.y_ttm--;
		else if(c == 'S') the_ball.y_ttm++;
	}
	set_ticker(0);
	endwin();
}
