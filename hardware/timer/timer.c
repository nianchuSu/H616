#include <stdio.h>		// standard C library
#include <sys/time.h>	// time function
#include <stdlib.h>		// exit
#include <signal.h>		// signal

static int i;	// static global variables	// the value is automatically initialized depending on the type

void fun1(int signum)	// signal handler
{
	i++;	// the interrupt function increments itself once it enters
	if(i == 2000){	// at 2000 times,the counter is reinitialized to 0 and other operations such as printing are performed
		printf("I am name nianxing_su.\n");
		/*the following three lines are to determine the period of the timer*/
		struct timeval t1;	// 1
		gettimeofday(&t1,NULL);	// 2
		printf("s:%ld\tus:%ld\n",t1.tv_sec,t1.tv_usec);		// 3	
		i = 0;
	}
}


int main()
{
	struct itimerval time;		// timer structure
	
	time.it_interval.tv_sec = 0;		// set initial timer value(seconds)
	time.it_interval.tv_usec = 500;		// set initial timer value(microsecond)

	time.it_value.tv_sec = 3;		// time to start timer(seconds)
	time.it_value.tv_usec = 0;		// time to start timer(microsecond)

	if(-1 == setitimer(ITIMER_REAL,&time,NULL)){	// timer function
		perror("setitimer");
		exit(-1);
	}	

	signal(SIGALRM,fun1);	// timer processing function
	
	while(1);		// card program to prevent program end
	return 0;
}
