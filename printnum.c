#include <stdio.h>
#include <pthread.h>

int counter = 0;

void *print_cnt(void *m)
{
	int i;
	for(i = 0; i < 5; i++){
		printf("counter = %d\n", counter);
		sleep(1);
	}
	return NULL;
}

int main()
{
	pthread_t tid1;
	int err, i;

	if((err = pthread_create(&tid1, NULL, print_cnt, NULL)) != 0)
		perror("can't creat thread 1");

	for(i = 0; i < 5; i++){
		counter++;
		sleep(1);
	}
	if((err = pthread_join(tid1, NULL)) != 0)
		perror("can't join thread 1");
}
