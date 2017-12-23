#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>

struct arg_set {
	char *fname;
	int counter;
};

pthread_mutex_t lock;
pthread_cond_t  flag;
struct arg_set *mailbox;

void *func(void *m)
{	
	FILE *fp;
	int c, precv = '\0';
	struct arg_set *args = (void *)m;

	if((fp = fopen(args->fname, "r")) != NULL){
		while((c = getc(fp)) != EOF){
			if(!isalnum(c) && isalnum(precv))
				args->counter++;
			precv = c;
		}
		fclose(fp);
	}
	else
		perror(args->fname);
	
	printf("COUNT: waiting to get lock\n");
	pthread_mutex_lock(&lock);
/*
	if(mailbox != NULL){
	
		printf("COUNT: the mailbox is used, waiting\n");
		pthread_cond_wait(&flag, &lock);
		printf("COUNT: the mailbox is released\n");
	
	}*/
	mailbox = args;

	printf("COUNT: unlocking box\n");
	pthread_mutex_unlock(&lock);

	printf("COUNT: raising flag\n");
	pthread_cond_signal(&flag);
	
	return NULL;
}

int main(int argc, char *argv[])
{
	struct arg_set args1, args2;
	pthread_t tid1, tid2;
	int err, two_thread, total_words;

	if(argc != 3){
		printf("usage: %s filename1 filename2\n", argv[0]);
		exit(1);
	}
	
	args1.fname = argv[1];
	args1.counter = 0;
	if((err = pthread_create(&tid1, NULL, func, (void *)&args1)) != 0)
		perror("creat thread 1 error");	
	
	args2.fname = argv[2];
	args2.counter = 0;
	if((err = pthread_create(&tid2, NULL, func, (void *)&args2)) != 0)
		perror("creat thread 2 error");	

	pthread_mutex_lock(&lock);

	while(two_thread < 2){
		printf("MAIN: waiting for flag to go up\n");
		pthread_cond_wait(&flag, &lock);

		printf("MAIN: WOW! the flag was raised, i have the lock\n");
		printf("%s : %d\n", mailbox->fname, mailbox->counter);
		total_words += mailbox->counter;

		if(mailbox == &args1)
			pthread_join(tid1, NULL);	
		if(mailbox == &args2)
			pthread_join(tid2, NULL);	
//		mailbox = NULL;
//		pthread_cond_signal(&flag);
		two_thread++;
	}
	printf("total: %d\n", total_words);
}
