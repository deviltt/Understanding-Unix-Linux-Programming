#include <stdio.h>
#include "stdlib.h"

#define oops(s, x) {perror(s); exit(x);}

void do_dc(int in[2], int out[2])
{
	if(dup2(in[0], 0) == -1)
		oops("direct stdin error", 5);
	close(in[0]);
	close(in[1]);

	if(dup2(out[1], 1) == -1)
		oops("direct stdout error", 6);
	close(out[0]);
	close(out[1]);

	execlp("dc", "dc", "-", NULL);
	oops("cannot run dc", 7);
}	

void do_bc(int bd[2], int db[2])
{
	int num1, num2;
	char message[BUFSIZ], operation[BUFSIZ];
	FILE *fpout, *fpin;

	close(bd[0]);
	close(db[1]);
	fpout = fdopen(bd[1], "w");
	fpin  = fdopen(db[0], "r");

	if(fpout == NULL || fpin == NULL)
		oops("err to convert", 3);

	while(printf("tinybc: "), fgets(message, BUFSIZ, stdin) != NULL){
		if(sscanf(message, "%d %[- + * / ^] %d", &num1, operation, &num2) != 3){
			printf("syntax error\n");
			continue;
		}
		
		if(fprintf(fpout, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF){
			printf("writing error\n");
			exit(4);
		}
		fflush(fpout);
		if(fgets(message, BUFSIZ, fpin) == NULL)
			break;
		printf("%d %c %d = %s", num1, *operation, num2, message);
	}	
	fclose(fpout);
	fclose(fpin);
}

int main()
{
	int pid, btod[2], dtob[2];
	
	if(pipe(btod) == -1 || pipe(dtob) == -1)
		oops("pipe error", 1);	
	if((pid = fork()) == -1)
		oops("fork error", 2);
	if(pid == 0)
		do_dc(btod, dtob);
	else{
		do_bc(btod, dtob);
		wait(NULL);
	}
}
