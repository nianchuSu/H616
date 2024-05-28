#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>

#define BEEP 2	// 定义针脚2是IO口


int main(void)
{
	wiringPiSetup();	// 初始化库
	
	pinMode(2, OUTPUT);	// 设置IO口的输入输出
	
	while(1){
		digitalWrite(2, HIGH);	// 低电平
		sleep(1);
		digitalWrite(2, LOW);	// 高电平
		sleep(1);
	}
	return 0;
}
