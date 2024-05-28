#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <unistd.h>
#include <stdlib.h>

int fd ;

void* wrhandler()
{
	char *buf,*p;
	buf = (char*)malloc(128*sizeof(char));
	p = buf;
	while(1){
		memset(buf,'\0',128);
		scanf("%s",buf);
		while(*buf){
    		serialPutchar (fd, *buf);
			buf++;
		}
		buf = p;
	}
}

void* rehandler()
{
	while(1){
    	while (serialDataAvail (fd)){
      		printf ("%c", serialGetchar (fd)) ;
      		fflush (stdout) ;
    	}
	}
}


int main ()
{

//  if ((fd = serialOpen ("/dev/ttyS2", 115200)) < 0)
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
	pthread_t t1,t2;

	pthread_create(&t1,NULL,wrhandler,NULL);
	pthread_create(&t2,NULL,rehandler,NULL);

for(;;);
  printf ("\n") ;
  return 0 ;
}
