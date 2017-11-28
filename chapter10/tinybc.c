#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#define oops(s,x) {perror(s); exit(x);}

void be_dc(int in[2], int out[2])
{
	if(dup2(in[0], 0) == -1)
		oops("dc:can't redirect stdin", 3);
	close(in[0]);
	close(in[1]);

	if(dup2(out[1], 1) == -1)
		oops("dc:can't redirect stdout", 4);
	close(out[0]);
	close(out[1]);

	execlp("dc", "dc", "-", NULL);
	oops("can't run dc", 5);
}

void be_bc(int todc[2], int fromdc[2])
{
	char operation[BUFSIZ], message[BUFSIZ];
	int num1, num2;
	FILE *fpout, *fpin;
	close(todc[0]);
	close(fromdc[1]);
	fpout = fdopen(todc[1], "w");
	fpin  =	fdopen(fromdc[0], "r");
	if(fpout == NULL || fpin == NULL){
		fprintf(stderr, "Error:error convering pipes to streams\n");
		exit(1);
	}
	while(printf("tinybc:"), fgets(message, BUFSIZ, stdin) != NULL){
		if(sscanf(message, "%d%[-+*/^]%d",&num1, operation, &num2) != 3){
			printf("syntax error\n");
			continue;
		}
		if(fprintf(fpout, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF){
			fprintf(stderr, "Error:error writing\n");
			exit(1);
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
	int pid, todc[2], fromdc[2];
	if(pipe(todc) == -1 || pipe(fromdc) == -1)
		oops("pipe error", 1);

	if((pid = fork()) == -1)
		oops("fork error", 2);
	if(pid == 0)
		be_dc(todc, fromdc);
	else{
		be_bc(todc, fromdc);
		wait(NULL);
	}
}
