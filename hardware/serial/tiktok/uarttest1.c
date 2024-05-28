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
				case 'N':
					printf("next\n");	// Output read content
					break;
				case 'L':
					printf("last\n");	// Output read content
					break;
				case 'P':
					printf("praise\n");	// Output read content
					break;
				case 'C':
					printf("close\n");	// Output read content
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
