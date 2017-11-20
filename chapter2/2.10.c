#include <stdio.h>
#include <unistd.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>

char *whats_myline(int fd)
{
	char *rv;
	if(rv = ttyname(0))
		if(strncmp(rv, "/dev/", 5) == 0)
			rv += 5;
	return rv;
}

void showtime(long timeval)
{
	char *cp;

	cp = ctime(&timeval);

	printf("%12.12s", cp + 4);
}

void show_info(struct utmp *utbufp)
{
	if(utbufp->ut_type != USER_PROCESS)
		return;

	printf("%-8.8s", utbufp->ut_name); 	//用户名
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);	//设备名字	
	printf(" ");
	showtime(utbufp->ut_time);
	printf(" (%s)", utbufp->ut_host);
	printf("\n");
}

int main(int argc, char *argv[])
{
	struct utmp utbuf;
	int utmpfd;
	char *myline = NULL;

	if(argc == 4)
		myline = whats_myline(0);

	if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
		perror(UTMP_FILE);
		exit(1);
	}

	while(read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
		if(myline == NULL || strcmp(utbuf.ut_line, myline) == 0)
			show_info(&utbuf);

	close(utmpfd);

	return 0;
}






