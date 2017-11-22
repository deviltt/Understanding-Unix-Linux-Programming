#include "stdio.h"
#include "termios.h"

#define TIMEOUT 6
#define ASK "will you take money?"
#define BEEP putchar('\a')

void set_no_echo(int timeout)
{
	struct termios ttyset;
	tcgetattr(0, &ttyset);		//读取tty驱动程序的属性
	ttyset.c_lflag &= ~ICANON;
	ttyset.c_lflag &= ~ECHO;
	ttyset.c_cc[VMIN] = 0;
	ttyset.c_cc[VTIME] = 10*timeout;	
	tcsetattr(0, TCSANOW, &ttyset);	//设置tty驱动程序的属性
}

int get_response(char *ask)
{	
	int input;
	printf("%s.(y/n)",ask);
	fflush(stdout);			//强制将缓冲区的数据写入stdout
	while(1){
		input = tolower(getchar());
		if(input == 'y')
			return 0;
		if(input == 'n')
			return 1;
		if(input == EOF)
			return 2;
		BEEP;
	}
}

int main()
{
	int response;
	set_no_echo(TIMEOUT);
	response = get_response(ASK);
	putchar('\n');
	return response;
}
