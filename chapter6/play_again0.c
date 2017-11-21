#include "stdio.h"
#include "termios.h"
#define QUESTION "Do you want another transaction"

int get_response(char *question)
{
	int input;
	printf("%s (y/n)?", question);
	while(1){
		switch(input = getchar()){
			case 'y':
			case 'Y': return 0;
			case 'n':
			case 'N':
			case EOF: return 1;
			default : 
				  printf("\ncan't understand %c, ", input);
				  printf("please type y or n \n");
		}
	}
}

/*
int tty_mode(int how)
{
	static struct termios original_mode;
	if(how == 0)
		tcgetattr(0, &original_mode);
	else
		return tcsetattr(0, TCSANOW, &original_mode);
}
*/
void set_crmode()
{
	struct termios ttystate;
	tcgetattr(0, &ttystate);		//获取标准输入tty的属性
	ttystate.c_lflag    &= ~ICANON;
	ttystate.c_cc[VMIN]  = 1;		//告诉程序一次可以读取多少字符
	tcsetattr(0, TCSANOW, &ttystate);
}

int main()
{
	int response;
	//tty_mode(0);
	set_crmode();
	response = get_response(QUESTION);
	printf("\n");
	return response;
}
