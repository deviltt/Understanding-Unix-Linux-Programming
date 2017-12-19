#include <stdio.h>
#include <pthread.h>

void *print_msg(void *m)
{
	char *cp = (char *)m;
	int i;
	
	for(i = 0; i < 5; i++){
		printf("%s", cp);
		fflush(stdout);
		sleep(1);
	}
	return NULL;
}

int main()
{
	int err;
	pthread_t tid1, tid2;
	
	if((err = pthread_create(&tid2, NULL, print_msg, (void *)"world\n")) != 0)
		perror("can't creat thread 1");
	if((err = pthread_create(&tid2, NULL, print_msg, (void *)"hello")) != 0)
		perror("can't creat thread 2");

	if((err = pthread_join(tid1, NULL)) != 0)
		perror("can't join thread 1");
	if((err = pthread_join(tid2, NULL)) != 0)
		perror("can't join thread 2");
}
