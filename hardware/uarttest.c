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


/*
设备:红米Redim 12 5G
屏幕:6.79
分辨率:2460*1080
*/
void* refun()		// Read-receive data
{
	char buf;	// cache region
	while(1){
		while((buf = uart_getc(fd)) > 0){	// Read the data and store it in the buffer
			switch(buf){
				case 'N':
					printf("next\n");	// Output read content
					system("adb shell input swipe 540 1300 540 500 100");
					break;
				case 'L':
					printf("last\n");	// Output read content
					system("adb shell input swipe 540 500 540 1300 100");
					break;
				case 'P':
					printf("praise\n");	// Output read content
					system("adb shell input tap 1000 1400");	// 单机点赞按钮/取消点赞
					break;
				case 'C':
					printf("close\n");	// Output read content
				//	system("adb shell input keyevent 3");
					system("adb shell input keyevent 82");	// 菜单键
					usleep(800000);	// 延时,防止程序未关闭
					system("adb shell input swipe 300 1300 0 1300 100");	// 左滑动
				//	usleep(100000);
					system("adb shell input keyevent 3");	// home键
					
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
