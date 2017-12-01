#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"

struct arg_set {
	char *fname;
	int count;
};

void *count_words(void *a)
{
	struct arg_set *args = a;
	FILE *fp;
	int c, prevc = '\0';

	if((fp = fopen(args->fname, "r")) !=NULL){
		while((c = getc(fp)) != EOF){
			if(! isalnum(c) && isalnum(prevc))
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
	pthread_t t1, t2;
	struct arg_set arg1, arg2;

	if(argc != 3){
		printf("usage: ./out file1 file2\n");
		exit(1);
	}
	arg1.fname  = argv[1];
	arg1.count = 0;
	pthread_create(&t1, NULL, count_words, (void *)&arg1);

	arg2.fname  = argv[2];
	arg2.count = 0;
	pthread_create(&t2, NULL, count_words, (void *)&arg2);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	
	printf("%s: %5d\n", arg1.fname, arg1.count);
	printf("%s: %5d\n", arg2.fname, arg2.count);
	printf("total: %5d\n", arg1.count + arg1.count);
	
	return 0;
}
