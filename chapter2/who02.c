#include "stdio.h"
#include "unistd.h"
#include "utmp.h"
#include "fcntl.h"
#include "time.h"

void show_info(struct utmp *utbufp);
void showtime(long timeval);

int main()
{
	struct utmp utbuf;
	int utmpfd;

	if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
		perror(UTMP_FILE);
		exit(1);
	}

	while(read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
		show_info(&utbuf);
	
	close(utmpfd);

	return 0;
}

void show_info(struct utmp *utbufp)
{
	if(utbufp->ut_type != USER_PROCESS) //USER_PROCESS为7，表示已经登录的用户
		return;

	printf("%-8.8s", utbufp->ut_name);
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);
	printf(" ");
	showtime(utbufp->ut_time);
	printf("\n");
}

void showtime(long timeval)
{
	char *cp;
	cp = ctime(&timeval);

	printf("%12.12s", cp + 4);
}




















