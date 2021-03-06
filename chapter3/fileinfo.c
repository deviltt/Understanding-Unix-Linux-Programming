#include "stdio.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "pwd.h"
#include "grp.h"
#include "dirent.h"

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

char *uid_to_name(uid_t uid)
{
	static char numstr[10];
	struct passwd *getpwuid(), *pw_ptr;

	if((pw_ptr = getpwuid(uid)) == NULL){
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid)
{
	static char numstr[10];
	struct group *getgrgid(), *grp_ptr;

	if((grp_ptr = getgrgid(gid)) == NULL){
		sprintf(numstr, "%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}

void show_stat_info(char *fname, struct stat *buf)
{
	showmode(buf->st_mode);
	printf("%d ", buf->st_nlink);
	printf("%s ", uid_to_name(buf->st_uid));   //将st_uid传递给getpwuid(),返回一个passwd结构体指针
	printf("%s ", gid_to_name(buf->st_gid));   //将st_gid传递给getgrgid(),返回一个group结构体指针
	printf("%d ", buf->st_size);
	showtime(buf->st_mtime);
	printf("%s\n", fname);

}

void dostat(char *filename)
{
	struct stat info;
	
	if(stat(filename, &info) == -1)
		perror(filename);
	else
		show_stat_info(filename, &info);
}

void do_ls(char dirname[])
{
	DIR *dir_ptr;
	struct dirent *direntp;

	if((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls1:cannot open %s\n", dirname);
	else{
		while((direntp = readdir(dir_ptr)) != NULL)
			dostat(direntp->d_name);
		close(dir_ptr);
	}
}

int main(int argc, char *argv[])
{
	if(argc == 1)
		do_ls(".");
	else
		while(--argc){
			printf("%s:\n", *++argv);
			do_ls(*argv);
		}
}
