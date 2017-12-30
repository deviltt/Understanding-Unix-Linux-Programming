#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define oops(x) {perror(x); exit(1);}

void do_dc(int in[2], int out[2])
{
	if(dup2(in[0], 0) == EOF)
		oops("dup2 error");
	close(in[0]);
	close(in[1]);

	if(dup2(out[1], 1) == EOF)
		oops("dup2 error");
	close(out[0]);
	close(out[1]);
	
	execlp("dc", "dc", "-", NULL);
}

void do_bc(int out[2], int in[2])
{
	FILE *fp_in, *fp_out;
	char message[BUFSIZ], operation[BUFSIZ];
	int num1, num2;

	close(out[0]);
	close(in[1]);

	fp_out = fdopen(out[1], "w");
	fp_in  = fdopen(in[0], "r");

	if(fp_out == NULL || fp_in == NULL)
		oops("fdopen error");

	while(printf("tinybc: "), fgets(message, BUFSIZ, stdin) != NULL){
		if(sscanf(message, "%d %[-+*/^] %d", &num1, operation, &num2) != 3){
			printf("syntax error\n");
			continue;
		}
	
		if(fprintf(fp_out, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF)
			oops("fprintf error");
		fflush(fp_out);

		if(fgets(message, BUFSIZ, fp_in) == NULL)
			break;	
		
		printf("%d %c %d = %s", num1, *operation, num2, message);
	}
	fclose(fp_out);
	fclose(fp_in);
}

int main()
{
	int pid;
	int to_dc[2], from_dc[2];

	if(pipe(to_dc) == EOF || pipe(from_dc) == EOF)
		oops("pipe error");

	if((pid = fork()) == EOF)
		oops("fork error");

	if(pid == 0){
		do_dc(to_dc, from_dc);
	}
	else{
		do_bc(to_dc, from_dc);
		wait(NULL);
	}
}
