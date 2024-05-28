#include <stdio.h>		// 标准库
#include <sys/time.h>	// gettimeofday

void count()
{
	int i = 0;

	for(i;i<100*1000;i++);		// if for 只执行一行代码可以这样操作
}

int main()
{
	struct timeval tvstart;
	struct timeval tvend;
	
	gettimeofday(&tvstart, NULL);	// Get the time since January 1, 1970(获取距1970年1月1日0时0分0秒的时间)
	count();	// 执行函数
	gettimeofday(&tvend, NULL);	// Get the time since January 1, 1970(获取距1970年1月1日0时0分0秒的时间)

	long t_us = 1000000 * (tvend.tv_sec - tvstart.tv_sec) + (tvend.tv_usec - tvstart.tv_usec);	// Calculate microsecond time(计算微秒时间)

	printf("Elapsed time1:%ldus\n",t_us);	// print (打印) 

	return 0;
}
