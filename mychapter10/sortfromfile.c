#include <stdio.h>
#include <fcntl.h>

#define oops(s,x) {perror(s); exit(x);}

int main(int argc, char *argv[])
{
	int fd;
	
	if(argc != 2){
		printf("usage: ./.out file\n");
		exit(1);
	}

	close(0);
	if(open(argv[1], O_RDONLY) != 0)
		oops("open error", 1);
	execlp("sort", "sort", NULL);
	return 0;
}
