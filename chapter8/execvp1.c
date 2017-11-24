#include "stdio.h"

void main()
{
	char *argvlist[3];
		
	argvlist[0] = "ls";
	argvlist[1] = "-l";
	argvlist[2] = 0;

	printf("*** About to exec ls -l\n");
	execvp("ls", argvlist);
	printf("*** ls is done. bye\n");
}
