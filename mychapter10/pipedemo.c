#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int len, i, apipe[2];
	char buf[BUFSIZ];

	if(pipe(apipe) == EOF){
		perror("pipe error");
		exit(1);
	}

	printf("pipein: %d, pipeout: %d\n", apipe[0], apipe[1]);

	while(fgets(buf, BUFSIZ, stdin)){
		len = strlen(buf);
		for(i = 0; i < len; i++)
			buf[i] = ' ';
		if(write(apipe[1], buf, len) != len){
			perror("write error");
		 	break;
		}

//		for(i = 0; i < len; i++)
//			buf[i] = ' ';

		if((len = read(apipe[0], buf, BUFSIZ)) == EOF){
			perror("read error");
			break;
		}
		if(write(1, buf, len) != len){
			perror("write error");	
			break;
		}
	}
	return 0;
}
