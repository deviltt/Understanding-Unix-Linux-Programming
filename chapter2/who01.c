#include "stdio.h"
#include "fcntl.h"
#include "utmp.h"
#include "unistd.h"

void show_info(struct utmp *utmpfp)
{
	printf("%-8.8s ",utmpfp->ut_user);
	printf("%-8.8s ",utmpfp->ut_line);
	printf("%10ld ",utmpfp->ut_time);
	printf("(%s)",utmpfp->ut_host);
	printf("\n");
}

int main()
{
	struct utmp current_record;
	int reclen = sizeof(struct utmp);
	int utmpfp;
	
	if((utmpfp = open(UTMP_FILE, O_RDONLY)) == -1){
		perror(UTMP_FILE);
		exit(1);
	}

	while(read(utmpfp, &current_record, reclen) == reclen)
		show_info(&current_record);
	
	close(utmpfp);
	
	exit(0);
}
