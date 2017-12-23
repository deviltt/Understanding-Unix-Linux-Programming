#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define oops(s) {perror(s); exit(1);}

void from_main(int in[2], int out[2])
{
	dup2(in[0], 0);
	close(in[0]);
	close(in[1]);

	dup2(out[1], 1);
	close(out[1]);
	close(out[0]);
}

void comefrom_dc(int out[2], int in[2])
{
	FILE *fp_out, *fp_in;
	char buf[BUFSIZ], operation[BUFSIZ];
	int num1, num2;

	close(out[0]);
	close(in[1]);

	fp_out = fdopen(out[1], "w"); 
	fp_in = fdopen(in[0], "r");
	if(fp_out == NULL && fp_in == NULL)
		perror("fdopen error");
	
	//printf("please give two numbers:\n");

	while(printf("tiny: "), fgets(buf, BUFSIZ, stdin) != NULL){
		if(sscanf(buf, "%d %[+-*/^] %d", &num1, operation, &num2) != 3){
			printf("scanf error, try again:\n");
			continue;
		}	

		if(fprintf(fp_out, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF)
			perror("fprintf error");
		fflush(fp_out);

		if(fgets(buf, BUFSIZ, fp_in) == NULL)
			break;

		printf("%d %c %d = %s", num1, *operation, num2, buf);
	}
	
	fclose(fp_in);
	fclose(fp_out);
}

int main()
{
	int to_dc[2], from_dc[2];
	int pid;

	if(pipe(to_dc) == EOF || pipe(from_dc) == EOF)
		perror("pipe 1 error");
	
	if((pid = fork()) < 0)
		perror("fork error");
	else if(pid == 0){
		from_main(to_dc, from_dc);
		execlp("dc", "dc", "-", NULL);
	}

	comefrom_dc(to_dc, from_dc);
	wait(NULL);
}
