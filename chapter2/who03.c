#include "stdio.h"
#include "fcntl.h"
#include "sys/types.h"
#include "utmp.h"
#include "unistd.h"
#include "time.h"

#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];
static int fd_utmp = -1;
static int cur_rec;				//跟踪当前位置
static int num_recs;				//一共有多少utmp结构体

int utmp_open(char *filename)
{
	fd_utmp = open(filename, O_RDONLY);  	//打开utmp.h文件
	cur_rec = num_recs = 0;  		//初始化
	return fd_utmp;                      	//返回文件描述符
}

struct utmp* utmp_next()
{
	struct utmp *recp;			//结构体指针，跟踪当前缓冲区的位置
	
	if(fd_utmp == -1)			//如果utmp.h文件没有打开，则返回空指针
		return NULLUT;
	if(cur_rec == num_recs && utmp_reload() == 0) //开始时cur_rec = 0; num_recs = 16
		return NULLUT;

	recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE]; //跟踪所指缓冲区的位置
	cur_rec++;
	return recp;
}

int utmp_reload()
{
	int utmp_read;
	
	utmp_read = read(fd_utmp, utmpbuf, UTSIZE * NRECS); //读取fd_utmp所指的文件，读取16个utmp
	num_recs = utmp_read / UTSIZE;		//num_recs = 16
	cur_rec = 0;
	return num_recs;
}

void utmp_close()
{
	if(fd_utmp != -1)
		close(fd_utmp);
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
		return ;
	
	printf("%-8.8s", utbufp->ut_name);
	printf(" ");	
	printf("%-8.8s", utbufp->ut_line);
	printf(" ");
	showtime(utbufp->ut_time);
	printf("\n");
}

int main()
{
	struct utmp *utbufp, *utmp_next();  	//结构体指针
	
	if(utmp_open(UTMP_FILE) == -1){    	//打开utmp.h文件
		perror(UTMP_FILE);
		exit(1);
	}
	
	while((utbufp = utmp_next()) != (struct utmp *)NULL) //遍历存贮16个utmp结构体的数组
		show_info(utbufp);  		//显示utmp里面的内容

	utmp_close();
	return 0;
}
