#include "stdio.h"
#include "curses.h"
#include "signal.h"
#include "sys/time.h"
#include "string.h"

char charset[] = "0123456789";

int dotest(int i)
{	
	int waitfor;
	int c, rv;
	struct timeval start, done;
	char digit;

	digit = charset[rand() % strlen(charset)];
	waitfor = rand() % 10;
	
	move(LINES / 2, COLS / 2 - 4);
	printw("TEST  %2d", i);
	move(LINES - 1, COLS - 1);
	refresh();
	sleep(waitfor);
	clear();
	refresh();
	move(LINES / 2, COLS / 2);
	addch(digit);
	refresh();

	gettimeofday(&start, 0);
	while((c = getch()) != EOF && c != digit);
	gettimeofday(&done, 0);

	move(LINES / 2, COLS / 2);
	addch(' ');
	refresh();

	if(c == EOF)
		rv = -1L;
	else
		rv = 1000000 * (done.tv_sec - start.tv_sec)
			     + (done.tv_usec - start.tv_usec);
	return rv;
}

void report(long data[], int n)
{
	int i;
	long sum;
 	long max, min;
	
	printf("%3d. %8.4f\n", 1, data[0] / 1000000.0);
	for(min = max = sum = data[0], i = 1; i < n; i++){
		printf("%3d. %8.4f\n", i+1, data[i] / 1000000.0);
		if(data[i] > max)
			max = data[i];
		if(data[i] < min)
			min = data[i];
		sum += data[i];
	}
	printf("max %8.4f    min %8.4f    avg %8.4f\n",
				max / 1000000.0, min / 1000000.0,
					(sum / (1000000.0 * n)));

}

int main()
{
	long results[10];
	int i;

	initscr();
	noecho();
	crmode();
	
	for(i = 0; i < 10; i++)
		results[i] = dotest(i + 1);
	endwin();	
	
	report(results, 10);
}
