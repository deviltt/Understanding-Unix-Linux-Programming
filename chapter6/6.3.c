#include "stdio.h"
#include "termios.h"
#include "fcntl.h"

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

void set_mode(int how)
{
	static struct termios original_mode;
	static int original_flag;
	if(how == 0){
		tcgetattr(0, &original_mode); 		//获取标准输入的tty的驱动信息
		original_flag = fcntl(0, F_GETFL);	//获取标准输入的文件标志
	}
	else{
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flag);
	}
}

int main()
{
	int response;
	set_mode(0);			//保存当前的状态信息，因为主程序会改变状态信息
	set_no_echo(TIMEOUT);
	response = get_response(ASK);
	set_mode(1);			//恢复主程序运行之前的状态信息
	putchar('\n');
	return response;
}
