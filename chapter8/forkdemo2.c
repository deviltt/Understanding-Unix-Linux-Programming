#include "stdio.h"

int main()
{
	printf("my pid is %d\n", getpid());

	fork();
	fork();
	fork();

	printf("my pif is %d\n", getpid());
}
