#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "wiringSerial.h"
#include "uarttool.h"
#include <pthread.h>

int fd;

void* wrfun()	// Send data
{
	char *buf;	// Pointer/string
	buf = (char*)malloc(128*sizeof(char));	// Open up memory space
	while(1){
		memset(buf,'\0',128);	// initialization string
		scanf("%s",buf);	// User input
		uart_Puts(fd, buf);	// Send cache data
	}
}

void* refun()		// Read-receive data
{
	char *buf;	// cache region
	buf = (char*)malloc(128*sizeof(char));	// Open up space
	while(1){
		while(uart_Getstr(fd,buf) > 0){	// Read the data and store it in the buffer
			printf("send->%s\n",buf);	// Output read content
			memset(buf,'\0',128);	// Clear cache
		}
	}
}


int main (int argc, char **argv)
{
	if(argc < 2){
		printf("Parameter missing");
		exit(-1);
	}

	char devicename[32] = {'\0'};

	strcpy(devicename,argv[1]);	// User parameter assignment

	fd = uart_Open(devicename,115200);
	if(fd == -1){
		perror("uart_Open");
		exit(-1);
	}
	pthread_t wr,re;	// Thread definition

	pthread_create(&wr,NULL,wrfun,NULL);	// Binding write thread
	pthread_create(&re,NULL,refun,NULL);	// Binding read thread


	for(;;);	// Prevent program exit
	return 0 ;
}
