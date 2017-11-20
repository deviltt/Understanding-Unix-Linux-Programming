#include "stdio.h"
#include "sys/types.h"
#include "sys/stat.h"

void showtime(long timeval)
{
	char *cp;
	cp = ctime(&timeval);
	
	printf("	%12.12s\n", cp + 4);
}

void show_stat_info(char *fname, struct stat *buf)
{
	
	printf("	mode    :  %o\n", buf->st_mode);
	printf("	links   :  %d\n", buf->st_nlink);
	printf("	user    :  %o\n", buf->st_uid);
	printf("	group   :  %d\n", buf->st_gid);
	printf("	size    :  %d\n", buf->st_size);
	showtime(buf->st_mtime);
	printf("	modtime :  %d\n", buf->st_mtime);
	printf("	name    :  %s\n", fname);

}

int main(int argc, char *argv[])
{
	struct stat info;
	
	if(argc > 1)
		if(stat(argv[1], &info) != -1){
			show_stat_info(argv[1], &info);
			return 0;
		}
		else
		 	perror(argv[1]);

	return 1;
}
