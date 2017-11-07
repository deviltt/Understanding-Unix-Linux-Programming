#include "stdio.h"

#define LINELEN 512
#define PAGELEN 24

void do_more(FILE *fp)
{
	char line[LINELEN];
	int num_of_line = 0;
	int reply;

	while(fgets(line, LINELEN, fp)){
		if(num_of_line == 24){
			reply = see_more();
			if(reply == 0)
				break;
			num_of_line -= reply;
		}
		if(fputs(line, stdout) == EOF)
			exit(1);
		num_of_line++;
	}
}

int see_more()
{
	int c;
	printf("\033[7m more?\033[m");
	while( (c = getchar()) != EOF){
		if(c == ' ')
			return PAGELEN;
		if(c == '\n')
			return 1;
		if(c == 'q')
			return 0;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	FILE *fp;
	if(argc == 1)
		do_more(stdin);
	else
		while(--argc){
			if((fp = fopen(*++argv, "r")) != NULL){
				do_more(fp);
				close(fp);
			}
			else
				exit(1);
		}
	return 0;
}
