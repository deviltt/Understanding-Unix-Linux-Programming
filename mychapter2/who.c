#include "stdio.h"
#include "utmp.h"
#include "fcntl.h"
#include "stdlib.h"
#include "time.h"

void showtime(long timeval)
{
	char *cp;
	cp = ctime(&timeval);

	printf("%12.12s", cp + 4);
}

void show_info(struct utmp *utmp_buf)
{
	if(utmp_buf->ut_type != USER_PROCESS)
		return;
	printf("%-8s", utmp_buf->ut_user);
	printf(" ");
	printf("%-8s", utmp_buf->ut_line);
	printf(" ");
//	printf("%10ld", utmp_buf->ut_time);
	showtime(utmp_buf->ut_time);
	printf("\n");
}

int main()
{
	int fd;
	struct utmp cur_pos;
	int strlen = sizeof(cur_pos);
	
	if((fd = open(UTMP_FILE, O_RDONLY)) == -1){
		perror("open error");
		exit(1);
	}
	while(read(fd, &cur_pos, strlen) == strlen)
		 show_info(&cur_pos);
	close(fd);
	exit(0);
}
