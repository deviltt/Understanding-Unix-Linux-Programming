#include "stdio.h"
#include "pthread.h"
#define num 5
void *print_msg(void *m)
{
	int i;
	for(i = 0; i < num; i++){
		printf("%s", (char *)m);
		fflush(stdout);
		sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t t1, t2;
	pthread_create(&t1, NULL, print_msg, (void *)"hello");
	pthread_create(&t2, NULL, print_msg, (void *)"world\n");
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
}
