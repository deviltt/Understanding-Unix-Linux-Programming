#include "stdio.h"
#include "sys/time.h"
#include "signal.h"

void countdown(int signum)
{
	static int num = 10;
	printf("%d..", num--);
	fflush(stdout);
	if(num < 0){
		printf("DONE! \n");
		exit(0);
	}
}

int set_ticker(int s_time, int r_time)
{
	struct itimerval new_timeset;
	long s_sec, s_usecs;
	long r_sec, r_usecs;

	s_sec = s_time / 1000;
	s_usecs = (s_time % 1000) * 1000L;
	r_sec = r_time / 1000;
	r_usecs = (r_time % 1000) * 1000L;
	
	new_timeset.it_interval.tv_sec  = r_sec;
	new_timeset.it_interval.tv_usec = r_usecs;
	
	new_timeset.it_value.tv_sec     = s_sec;
	new_timeset.it_value.tv_usec    = s_usecs;
	
	return setitimer(ITIMER_REAL, &new_timeset, NULL);	//将时间传递给真实时间计时器
}

int main()
{
	signal(SIGALRM, countdown);
	if(set_ticker(5000,1000) == -1)		//5000为初始时间间隔，1000为重复时间间隔
		perror("set_ticker");
	else
		while(1)
			pause();
		return 0;
}
