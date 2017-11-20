#include "stdio.h"
#include "sys/types.h"
#include "sys/stat.h"

void showtime(long timeval)
{
	char *cp;
	cp = ctime(&timeval);
	
	printf("%12.12s ", cp + 4);
}

void showmode(long stmode)
{
	char str[10];
	strcpy(str, "----------");
	if(S_ISDIR(stmode)) str[0] = 'd';
	if(S_ISCHR(stmode)) str[0] = 'c';
	if(S_ISBLK(stmode)) str[0] = 'b';

	if(stmode & S_IRUSR) str[1] = 'r';
	if(stmode & S_IWUSR) str[2] = 'w';
	if(stmode & S_IXUSR) str[3] = 'x';

	if(stmode & S_IRGRP) str[4] = 'r';
	if(stmode & S_IWGRP) str[5] = 'w';
	if(stmode & S_IXGRP) str[6] = 'x';

	if(stmode & S_IROTH) str[7] = 'r';
	if(stmode & S_IWOTH) str[8] = 'w';
	if(stmode & S_IXOTH) str[9] = 'x';

	printf("%s ", str);
}

void show_stat_info(char *fname, struct stat *buf)
{
	showmode(buf->st_mode);
	printf("%d ", buf->st_nlink);
	printf("%o ", buf->st_uid);
	printf("%d ", buf->st_gid);
	printf("%d ", buf->st_size);
	showtime(buf->st_mtime);
	printf("%s\n", fname);

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
