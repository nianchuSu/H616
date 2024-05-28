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
	点击是一个功能
	滑动是一个功能
	功能键也是功能
	所有都可以封装成函数然后根据传参来动作
*/


void* refun()		// Read-receive data
{
	char buf;	// cache region
	while(1){
		while((buf = uart_getc(fd)) > 0){	// Read the data and store it in the buffer
			switch(buf){
				case 'e':	// 下一个
					printf("NO:1\n");
					system("adb shell input swipe 540 1300 540 500 100");
					break;
				case 'f':	// 上一个
					printf("NO:2\n");
					system("adb shell input swipe 540 500 540 1300 100");
					break;
				case 'g':	// 暂停
					printf("NO:3\n");
					system("adb shell input tap 500 1300");
					break;
				case 'h':	// 点赞
					printf("NO:4\n");
					system("adb shell input tap 500 1300");
					usleep(8000);
					system("adb shell input tap 500 1300");
					break;
				case 'i':	// 取消点赞
					printf("NO:5\n");
					system("adb shell input tap 1000 1400");
					break;
				case 'p':	// 关注
					printf("NO:6\n");
					system("adb shell input tap 995 1285");	// 进入主页
					// usleep(800000);usleep(800000);
					// system("adb shell input tap 300 1300");
					break;
				case 'q':	// 取消关注+返回视频页
					printf("NO:7\n");
					system("adb shell input tap 1000 1250");	// 进入主页
					usleep(800000);usleep(800000);
					system("adb shell input tap 300 1300");
					usleep(800000);
					system("adb shell input tap 950 2250");
					usleep(800000);
					system("adb shell input swipe 50 500 540 500 100");
					break;
				case 'r':	// 静音/取消静音
					printf("NO:8\n");
					system("adb shell input keyevent 164");
					break; 
				case 's':	// 加声音
					printf("NO:9\n");
					system("adb shell input keyevent 24");
					break;
				case 't':	// 减声音
					printf("NO:10\n");
					system("adb shell input keyevent 25");
					break;
				case 'u':	// 刷新所有视频
					printf("NO:11\n");
					system("adb shell input tap 150 2350");
					break;
				case 'H':	// 关闭程序
					printf("NO:0\n");
					system("adb shell input keyevent 82");	// 菜单键
					usleep(800000);	// 延时,防止程序未关闭
					system("adb shell input swipe 300 1300 0 1300 100");	// 左滑动
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
