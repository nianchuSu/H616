#include <stdio.h>	// Standard C library(标准C库)
#include <sys/time.h>	// gettimeofday()
#include <wiringPi.h>	// raspberry Pi/orangepi function library(树莓派/香橙派的函数库)
#include <unistd.h>		// usleep
#include <stdlib.h>		// exit

#define Trig 0
#define Echo 1

double get_sup()
{
	double dis = 0;		// define distance variable
	struct timeval start;	// start time
	struct timeval end;		// end time
	
	pinMode(Trig, OUTPUT);	// define trig as the output
	pinMode(Echo, INPUT);	// define echo as the input
	
	digitalWrite(Trig, LOW);	// trig is low by default
	usleep(10);		// hold for 10 microseconds
	
	digitalWrite(Trig, HIGH);	// trig to high level
	usleep(11);		// maintain high level for 11 microseconds 
	digitalWrite(Trig, LOW);	// trig goes low 

	while(!digitalRead(Echo));	// echo the default low level,take the reverse card cycle
	gettimeofday(&start,NULL);	// echo high gets the timestamp right away
	while(digitalRead(Echo));	// echo maintains a high level
	gettimeofday(&end,NULL);	// echo obtains the current timestamp at a low level

	
	long elapsedtime = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);	// computing time
	while(elapsedtime < 0);
	printf("%ld\n",elapsedtime);
//	dis = 340 * 100 * (elapsedtime /1000000.0) / 2;	// *100the velocity is converted to centimeters		/2round trip distance
	dis = 340 * 100 * ((double)(elapsedtime) /1000000) / 2;	// *100the velocity is converted to centimeters		/2round trip distance	 double type coercion

	return dis;		// return distance
}

int main()
{
	if(wiringPiSetup()  == -1){
		perror("wiringPiSetup");
		
		exit(-1);
	}
	double dis;	// distance
	while(1){
		dis = get_sup();	// Gets the distance returned by the ultrasonic module(获取超声波模块返回的距离)
		printf("dis = %lf\n",dis);	// print
		usleep(500000);		// wait 500 milliseconds
	}

	return 0;
}
