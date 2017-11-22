#include "stdio.h"
#include "termios.h"
#include "ctype.h"

#define TABWIDTH 20

void set_mode(int how)
{
	static struct termios original_mode;
	if(how == 0)
		tcgetattr(0, &original_mode);
	else
		tcsetattr(0, TCSANOW, &original_mode);
}

void get_control_chars(int *erasep, int *killp)
{
	struct termios ttystate;
	tcgetattr(0, &ttystate);
	*erasep = ttystate.c_cc[VERASE];
	*killp  = ttystate.c_cc[VKILL];
}

void set_tty_mode()
{
	struct termios tty_state;
	tcgetattr(0, &tty_state);
	tty_state.c_lflag &= ~ICANON;
	tty_state.c_lflag &= ~ECHO;
	tty_state.c_cc[VMIN] = 1;
	tty_state.c_lflag &= ~ISIG;
	tcsetattr(0, TCSANOW, &tty_state);
}

int main()
{
	int c;
	int erase, kill;
	int pos = 0;
	set_mode(0);
	get_control_chars(&erase, &kill);
	set_tty_mode();
	while((c = getchar()) != EOF && c != 'Q'){
		if(c == erase){
			if(pos > 0){
				printf("\b \b");	//后退一格打印空白此时光标又后移了一个，因此需要再后退一格
				pos--;
			}
			continue;
		}
		if(c == kill){
			while(pos > 0){
				printf("\b \b");
				pos--;
			}
			continue;
		}

		if(c == 'z')
			c = 'a';
		else if(c)
			c++;
		putchar(c);

		if(isprint(c))		//判断c是否为可打印字符
			pos++;
		else if(c = '\t')
			pos += (TABWIDTH - (pos % TABWIDTH));
		else if(c = '\n')
			pos = 0;
	}
	set_mode(1);
	return 0;
}
