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

void* refun()		// Read-receive data
{
	char buf;	// cache region
	while(1){
		while((buf = uart_getc(fd)) > 0){	// Read the data and store it in the buffer
			switch(buf){
				case 'A':
					printf("NO:1\n");
					
					break;
				case 'B':
					printf("NO:2\n");
					
					break;
				case 'C':
					printf("NO:3\n");
					
					break;
				case 'D':
					printf("NO:4\n");
					
					break;
				case 'E':
					printf("NO:5\n");
					
					break;
				case 'F':
					printf("NO:6\n");
					
					break;
				case 'G':
					printf("NO:7\n");
					
					break;
				case 'H':
					printf("NO:8\n");
					
					break;
				case 'I':
					printf("NO:9\n");
					
					break;
				case 'J':
					printf("NO:10\n");
					
					break;
				case 'K':
					printf("NO:11\n");
					
					break;
				case '0':
					printf("NO:0\n");
					
					break;
			}
			buf = -1;	// Clear cache
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
	pthread_t re;	// Thread definition

	pthread_create(&re,NULL,refun,NULL);	// Binding read thread


	for(;;);	// Prevent program exit
	return 0 ;
}
