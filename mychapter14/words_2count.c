#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>

struct arg_set {
	char *fname;
	int count;
};

void *func(void *a)
{
	struct arg_set *args = a;
	FILE *fp;
	int c, prevc = '\0';

	if((fp = fopen(args->fname, "r")) != NULL){
		while((c = getc(fp)) != EOF){
			if(!isalnum(c) && isalnum(prevc))
				args->count++;
			prevc = c;
		}
		fclose(fp);
	}
	else
		perror(args->fname);
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t tid1, tid2;
	struct arg_set args1, args2;
	int err;

	if(argc != 3){
		printf("usage: %s filename1 filename2\n", argv[0]);
		exit(1);
	}
	
	args1.fname = argv[1];
	args1.count = 0;
	if((err = pthread_create(&tid1, NULL, func, (void *)&args1)) != 0)
		perror("can't creat thread 1");
	
	args2.fname = argv[2];
	args2.count = 0;
	if((err = pthread_create(&tid2, NULL, func, (void *)&args2)) != 0)
		perror("can't creat thread 2");

	if((err = pthread_join(tid1, NULL)) != 0)
		perror("can't join thread 1");

	if((err = pthread_join(tid2, NULL)) != 0)
		perror("can't join thread 2");

	printf("total words: %d\n", args1.count + args2.count);
	printf("%s: %d\n", argv[1], args1.count);
	printf("%s: %d\n", argv[2], args2.count);

	return 0;
}
