#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>

#define BEEP 2	// 定义针脚2是IO口


int main(void)
{
	wiringPiSetup();	// 初始化库
	
	pinMode(2, OUTPUT);	// 设置IO口的输入输出

	char off = 'n';	// 定义开关

	while(1){
		digitalWrite(2, HIGH);	// 高电平
		sleep(1);
		digitalWrite(2, LOW);	// 低电平
		sleep(1);
		if(off){
			digitalWrite(2, HIGH);	// 高电平
			while(1){
				getc()
			}	
		}
	}
	return 0;
}
