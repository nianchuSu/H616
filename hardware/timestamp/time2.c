#include <stdio.h>		// 标准库
#include <sys/time.h>	// gettimeofday

void count()
{
	int i;
	int j;

	for(i=0;i<100;i++)		// if for 只执行一行代码可以这样操作
		for(j=0;j<1000;j++);	// 计数1000*1000
//	printf("i:%d,j:%d\n",i,j);
}

int main()
{
	struct timeval tvstart;
	struct timeval tvend;
	
	gettimeofday(&tvstart, NULL);	// Get the time since January 1, 1970(获取距1970年1月1日0时0分0秒的时间)
	count();	// 执行函数
	gettimeofday(&tvend, NULL);	// Get the time since January 1, 1970(获取距1970年1月1日0时0分0秒的时间)

	long t_us = 1000000 * (tvend.tv_sec - tvstart.tv_sec) + (tvend.tv_usec - tvstart.tv_usec);	// Calculate microsecond time(计算微秒时间)

	printf("Elapsed time2:%ldus\n",t_us);	// print (打印) 

	return 0;
}
