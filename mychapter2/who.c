#include "stdio.h"
#include "utmp.h"
#include "fcntl.h"
#include "stdlib.h"

void show_info(struct utmp *utmp_buf)
{
	printf("%-8s", utmp_buf->ut_user);
	printf(" ");
	printf("%-8s", utmp_buf->ut_line);
	printf(" ");
	printf("%10ld", utmp_buf->ut_time);
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
