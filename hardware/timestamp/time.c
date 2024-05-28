#include <stdio.h>		// 标准库
#include <sys/time.h>	// gettimeofday


int main()
{
	struct timeval tv;
	
	gettimeofday(&tv, NULL);	// Get the time since January 1, 1970(获取距1970年1月1日0时0分0秒的时间)

	printf("time:%lds,time:%ldus\n",tv.tv_sec,tv.tv_usec);	// print (打印) 

	return 0;
}
