#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <stdlib.h>

int total_words;

void *count_words(void *f)
{
	char *filename = (char *)f;
	FILE *fp;
	int c, n = 0, prevc = '\0';
	pthread_mutex_t lock;
	
	pthread_mutex_init(&lock, NULL);
/*
	if((fp = fopen(filename, "r")) != NULL){
		while((c = getc(fp)) != EOF){
			if(!isalnum(c)){
				n = 0;
			}
			else if(n == 0){
				total_words++;
				n = 1;
			}
		}
		fclose(fp);
	}*/
	if((fp = fopen(filename, "r")) != NULL){
		while((c = getc(fp)) != EOF){
			if(!isalnum(c) && isalnum(prevc)){
				pthread_mutex_lock(&lock);	
				total_words++;
				pthread_mutex_unlock(&lock);
			}
			prevc = c;
		}
		fclose(fp);
	}
	
	else
		perror("fopen error");
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t tid1, tid2;
	int err;

	if(argc != 3){
		printf("usage: %s file1 file2\n", argv[0]);
		exit(1);
	}

	total_words = 0;

	if((err = pthread_create(&tid1, NULL, count_words, (void *)argv[1])) != 0)
		perror("can't creat thread 1");
	if((err = pthread_create(&tid2, NULL, count_words, (void *)argv[2])) != 0)
		perror("can't creat thread 2");

	if((err = pthread_join(tid1, NULL)) != 0)
		perror("creat thread 1 error");
	if((err = pthread_join(tid2, NULL)) != 0)
		perror("creat thread 2 error");

	printf("total words: %5d\n", total_words);
}
