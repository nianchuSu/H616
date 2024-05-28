#include <stdio.h>
#include <sys/time.h>	// time function
#include <stdlib.h>		// exit
#include <signal.h>		// signal processing

static int i;	// global static variable
static int j = 1;	// global static variable

void sigfun1(int signum)
{
	i++;
	if(i == 1*1000){
		i = 0;
		printf("%d*************************\n",j++);
	}
}

int main()
{
	struct itimerval t1;	// time struct

	t1.it_interval.tv_sec = 0;	// timer initial	1s
	t1.it_interval.tv_usec = 1000;

	t1.it_value.tv_sec = 1;		// 	timer start time	start the timer when the program starts
	t1.it_value.tv_usec = 0;	// 	The starting time cannot be less than 200 microseconds

	int num = setitimer(ITIMER_REAL,&t1,NULL);	// timer function
	printf("%d\n",num);
	signal(SIGALRM,sigfun1);	// signal processing

	while(1);
	return 0;
}
