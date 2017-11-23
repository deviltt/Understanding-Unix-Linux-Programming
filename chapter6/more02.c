#include "stdio.h"
#include "signal.h"
#include "termios.h"

#define PAGELEN 24
#define LINELEN 512

int see_more(FILE *ttyfp)
{
	int c;
	printf("\033[7m more? \033[m");
	while((c = getc(ttyfp)) != EOF){
		printf("\r              \r");
		if(c == 'q')
			return 0;
		if(c == '\n')
			return 1;
		if(c == ' ')
			return PAGELEN;
		printf("\033[7m more? \033[m");
	} 
	return 0;
}

void do_more(FILE *fp)
{
	char line[LINELEN];
	int num_of_line = 0;
	int reply;
	FILE *ttyfp;

	ttyfp = fopen("/dev/tty", "r");
	if(ttyfp == NULL)
		exit(1);
	while(fgets(line, LINELEN, fp)){
		if(num_of_line == PAGELEN){
			reply = see_more(ttyfp);
			if(reply == 0)
				break;
			num_of_line -= reply;	
		}
		if(fputs(line, stdout) == EOF)
			exit(1);
		num_of_line++;
	}
}

void handler()
{
	void setting(int);
	settings(1);
	return 0;
}

void settings(int how)
{
	static struct termios tty_orig;
	struct termios tty_now;
	
	if(how == 0){
		tcgetattr(0, &tty_orig);
		tty_now = tty_orig;
		tty_now.c_lflag &= ~ICANON;
		tty_now.c_lflag &= ~ECHO;
		tty_now.c_cc[VMIN] = 1;
		tcsetattr(0, TCSANOW, &tty_now);	
		
		signal(SIGINT, handler);
		signal(SIGQUIT, handler);
	}
	else
		tcsetattr(0, TCSANOW, &tty_orig);
}

int main(int argc, char *argv[])
{
	FILE *fp;
	
	settings(0);
	if(argc == 1)
		do_more(stdin);
	else
		while(--argc){
			if((fp = fopen(*++argv,"r")) != NULL){
				do_more(fp);
				fclose(fp);
			}
			else
				exit(1);			
		}
	settings(1);
	return 0;
}
