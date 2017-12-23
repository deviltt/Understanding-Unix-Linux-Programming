#include <stdio.h>
#include <pthread.h>
#include <string.h>

void *print_msg(void *m)
{
	char *cp = (char *)m;
	int i, len;
	
	len = strlen(cp);
	for(i = 0; i < len; i++){
		printf("%s", cp);
		fflush(stdout);
		sleep(1);
	}
	return NULL;
}

int main()
{
	int err;
	pthread_t tid1, tid2, tid3, tid4;
	
	if((err = pthread_create(&tid1, NULL, print_msg, (void *)"world\n")) != 0)
		perror("can't creat thread 1");
	if((err = pthread_create(&tid2, NULL, print_msg, (void *)"hello\n")) != 0)
		perror("can't creat thread 2");
	if((err = pthread_create(&tid3, NULL, print_msg, (void *)"th\n")) != 0)
		perror("can't creat thread 3");
	if((err = pthread_create(&tid4, NULL, print_msg, (void *)"but\n")) != 0)
		perror("can't creat thread 4");
		

	if((err = pthread_join(tid1, NULL)) != 0)
		perror("can't join thread 1");
	printf("tid 1 return\n");
	if((err = pthread_join(tid2, NULL)) != 0)
		perror("can't join thread 2");
	printf("tid 2 return\n");
	if((err = pthread_join(tid3, NULL)) != 0)
		perror("can't join thread 3");
	printf("tid 3 return\n");
	if((err = pthread_join(tid4, NULL)) != 0)
		perror("can't join thread 4");
	printf("tid 4 return\n");
}
