#include "stdio.h"
#include "fcntl.h"
#include "termios.h"

void set_mode(int how)
{
	static struct termios original_mode;
	static int original_flag;
	if(how == 0){
		tcgetattr(0, &original_mode);
		original_flag = fcntl(0, F_GETFL);
	}
	else{
		tcsetattr(0, TCSANOW, &original_mode);
		fcntl(0, F_SETFL, original_flag);
	}
}

void set_tty_mode()
{
	struct termios ttystate;
	tcgetattr(0, &ttystate); 
	ttystate.c_lflag &= ~ICANON;  	//关闭规范模式
	ttystate.c_lflag &= ~ECHO;	//关闭回显
	ttystate.c_cc[VMIN] = 1;	
	ttystate.c_lflag &= ~ISIG; 	//禁止中断，退出等信号
	tcsetattr(0, TCSANOW, &ttystate);	//TCSANOW表示立即更新驱动程序设置
}

int main()
{	
	int input;
	set_mode(0);			//保留开始的tty设备信息以及文件标志信息
	set_tty_mode();			//按要求更改tty设备驱动信息
	while((input = getchar()) != EOF && input != 'Q'){	//获取输入的字符串并判断是否为Q
		if(input == 'z')
			input = 'a';
		else if(islower(input))
			input++;
		putchar(input);
	}
	set_mode(1);			//恢复开始保留的信息
	return 0;
}
