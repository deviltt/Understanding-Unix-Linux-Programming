#include "stdio.h"
#include "signal.h"
#include "termios.h"
#include "sys/ioctl.h"

char filler = 'A';

int screen_size(int *rowp, int *colp)
{
	struct winsize wbuf;

	if(ioctl(1, TIOCGWINSZ, &wbuf) != -1){
		*rowp = wbuf.ws_row;
		*colp = wbuf.ws_col;
		return 0;
	}
	return -1;
}

void fill_screen()
{
	int i,n;
	int rows, cols;
	if(screen_size(&rows, &cols) == 0){
		n = rows * cols;
		for(i = 0; i < n; i++)
			putchar(filler);
	}
	filler++;
}

void getout()
{
	void settings(int);
	settings(1);
	exit(0);
}

void settings(int how)
{
	struct termios tty_now;
	static struct termios tty_orig;

	if(how == 0){
		tcgetattr(0, &tty_orig);	
		tty_now = tty_orig;
		tty_now.c_lflag &= ~ICANON;
		tty_now.c_lflag &= ~ECHO;
		tty_now.c_cc[VMIN] =1;
		tcsetattr(0, TCSANOW, &tty_now);
		
		signal(SIGINT  , getout);
		signal(SIGQUIT , getout);
		signal(SIGWINCH, fill_screen);
	}
	else
		tcsetattr(0, TCSANOW, &tty_orig);
}

int main()
{
	int c;
	settings(0);
	fill_screen();
	while((c = getchar()) != EOF && c != 'Q'){
		filler = 'A';	//按任意不是Q的按键，从A重新开始
		fill_screen();
	}
	settings(1);
}
