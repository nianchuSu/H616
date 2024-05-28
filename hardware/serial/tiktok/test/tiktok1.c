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
//char tmp[128];

// 功能按键
void function_but(int num)
{	
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "adb shell input keyevent %d", num);
	system(tmp);
}

// 点击功能
void click(int x, int y)
{
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "adb shell input tap %d %d",x,y);
	system(tmp);	// 这个才对
//	system("adb shell input tap x y");	// 这个不对
}

// 双击/多次点击
void periodic()
{
	
}

// 滑动
void slide(int x, int xa, int y, int ya, int t)		// 五个参数x是起始值xa是末尾,t是时间单位毫秒
{
	
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "adb shell input swipe %d %d %d %d %d",x,y,xa,ya,t);
	system(tmp);
}

// 上下滑动
void slide_ub(int x, int y, int ya, int t)		// 上下滑动改变的是y的值,纵坐标
{
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "adb shell input swipe %d %d %d %d %d",x,y,x,ya,t);
	system(tmp);
}

// 左右滑动
void slide_lr(int x, int xa, int y, int t)		// 左右滑动改变的是x的值,横坐标
{
	char tmp[128];
	snprintf(tmp, sizeof(tmp), "adb shell input swipe %d %d %d %d %d",x,y,xa,y,t);
	system(tmp);
}

void* refun()		// Read-receive data
{
	char buf;	// cache region
	while(1){
		while((buf = uart_getc(fd)) > 0){	// Read the data and store it in the buffer
			switch(buf){
				case 'A':	// 下一个
					printf("NO:下一个\n");
					slide_ub(666,1345,345,150);
					break;
				case 'B':	// 上一个
					printf("NO:上一个\n");
					slide_ub(666,345,1345,150);
					break;
				case 'C':	// 暂停
					printf("NO:暂停\n");
					click(500,1300);
					break;
				case 'D':	// 点赞
					printf("NO:点赞\n");
					click(500,1300);
					usleep(8000);
					click(500,1300);
					break;
				case 'E':	// 取消点赞
					printf("NO:取消点赞\n");
					click(1000,1400);
					break;
				case 'F':	// 关注
					printf("NO:关注\n");
					click(995,1185);
					break;
				case 'G':	// 静音/取消静音
					printf("NO:静音/取消静音\n");
					function_but(164);
					break;
				case 'H':	// 加声音
					printf("NO:加声音\n");
					function_but(24);
					break;
				case 'I':	// 减声音
					printf("NO:减声音\n");
					function_but(25);
					break;
				case 'P':	// 刷新所有视频
					printf("NO:刷新所有视频\n");
					click(150,2350);
					break;
				case 'Q':	// 上滑动/下翻页
					printf("NO:上滑动/下翻页\n");
					slide_ub(666,1345,345,150);
					break;
				case 'R':	// 下滑动/上翻页
					printf("NO:下滑动/上翻页\n");
					slide_ub(666,345,1345,150);
					break;
				case 'S':	// 左滑动/向右
					printf("NO:左滑动/向右\n");
					slide_lr(150,800,1500,150);
					break;
				case 'T':	// 右滑动/向左
					printf("NO:右滑动/向左\n");
					slide_lr(800,150,1500,150);
					break;
				
				case '0':	// 关闭程序
					printf("NO:关闭程序\n");
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
