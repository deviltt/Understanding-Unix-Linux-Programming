#include "stdio.h"

int main(int argc, char *argv[])
{
	int i;
	for(i = 0; i < argc; i++){
		printf("argv[%d] %s\n",i ,argv[i]);
	}
	fprintf(stderr, "this message is sent to stderr\n");
}
