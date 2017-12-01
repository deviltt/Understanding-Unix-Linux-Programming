#include "stdio.h"
#include "pthread.h"
#include "ctype.h"
#include "stdlib.h"

struct arg_set{
	int count;
	char *fname;
};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t flag = PTHREAD_COND_INITIALIZER;

struct arg_set *mailbox;

void *count_words(void *a)
{
	struct arg_set *args = a;
	FILE *fp;
	int c, prevc = '\0';

	if((fp = fopen(args->fname, "r")) != NULL){
		while((c = getc(fp)) != EOF){
			if(! isalnum(c) && isalnum(prevc))
				args->count++;
			prevc = c;
		}
		fclose(fp);
	}else
		perror(args->fname);
	printf("COUNT: waiting to get lock\n");
	pthread_mutex_lock(&lock);
	printf("COUNT: have lock, storing data\n");
	if(mailbox != NULL)
		pthread_cond_wait(&flag, &lock);
	mailbox = args;
	printf("COUNT: raising flag\n");
	pthread_cond_signal(&flag);
	printf("COUNT: unlocking box\n");
	pthread_mutex_unlock(&lock);
	return NULL;
}

int main(int argc, char *argv[])
{
	struct arg_set arg1, arg2;
	int total_words = 0;
	pthread_t t1, t2;
	int reports_in = 0;

	if(argc != 3){
		perror("usage: ./out file1 file2\n");
		exit(1);
	}
	
	arg1.fname = argv[1];
	arg1.count = 0;
	pthread_create(&t1, NULL, count_words, (void *)&arg1);
	
	arg1.fname = argv[1];
	arg1.count = 0;
	pthread_create(&t1, NULL, count_words, (void *)&arg1);

	while(reports_in < 2){
		printf("MAIN: waiting for flag to go up\n");
		pthread_cond_wait(&flag, &lock);
		printf("MAIN: Wow! flag was raised, I have the lock\n");
		printf("%7d: %s\n", mailbox->count, mailbox->fname);
		total_words += mailbox->count;
		if(mailbox == &arg1)
			pthread_join(t1, NULL);
		if(mailbox == &arg2)
			pthread_join(t2, NULL);
		mailbox = NULL;
		pthread_cond_signal(&flag);
		reports_in++;
	}
	printf("%7d: total words\n", total_words);
}

