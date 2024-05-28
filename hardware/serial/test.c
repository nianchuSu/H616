#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdlib.h>
#include <unistd.h>

int fd;

void* handlerwrite()
{
	char *p;
	char *buf = (char*)malloc(128 * sizeof(char));
	p = buf;
	memset(buf,'\0',128);
	while(1){
		scanf("%s",buf);
		while(*buf){	
			serialPutchar (fd, *buf) ;
			buf++;
		}
		buf = p;
		memset(buf,'\0',128);
	}
}

void* handlerread()
{
	while(1){
		while(serialDataAvail(fd)){
			printf (" -> %c\n", serialGetchar (fd)) ;
			fflush (stdout) ;
		}
	}	
}


int main ()
{
	if ((fd = serialOpen ("/dev/ttyS5", 115200)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}

	if (wiringPiSetup () == -1)
	{
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		return 1 ;
	}

	pthread_t idwrite;
	pthread_t idread;

	pthread_create(&idwrite,NULL,handlerwrite,NULL);
	pthread_create(&idread,NULL,handlerread,NULL);

	while(1)
		sleep(10);

	return 0 ;
}
