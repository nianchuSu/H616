#include <stdio.h>
#include <sys/time.h>	// time function
#include <stdlib.h>		// exit
#include <signal.h>		// signal processing
#include <wiringPi.h>	// peripheral library

#define sg90 5

static int i = 0;	// global static variable
int angle =0;
void sigfun1(int signum)
{
	if(i <= angle){
		digitalWrite(sg90,HIGH);
	}else{
		digitalWrite(sg90,LOW);
	}
	if(i == 40){
		i = 0;
	}		
	i++;
}

void inittimer()
{
	struct itimerval t1;	// time struct

	t1.it_interval.tv_sec = 0;	// timer initial	1s
	t1.it_interval.tv_usec = 500;

	t1.it_value.tv_sec = 1;		// 	timer start time	start the timer when the program starts
	t1.it_value.tv_usec = 0;	// 	The starting time cannot be less than 200 microseconds

	if(-1 == setitimer(ITIMER_REAL,&t1,NULL)){	// timer function
		perror("setitimer");
		exit(-1);
	}
	
}


int main()
{
	wiringPiSetup();	// initializes the peripheral library
	pinMode(sg90,OUTPUT);	// defines the working mode of the I/O port
	angle = 1;

	inittimer();		
	signal(SIGALRM,sigfun1);	// signal processing

	while(1){
		printf("请输入转动角度:1-0°  2-45°  3-90°  4-135° 5-180°  0退出\n");
		scanf("%d",&angle);
		if(angle == 0){
			break;
		}
	}
	return 0;
}
