#include <stdio.h>		// 标准库
#include <sys/time.h>	// gettimeofday

void count()
{
	int i = 0;

	for(i;i<100;i++){		// if for 只执行一行代码可以这样操作
//		printf("i:%d\n",i);
		int j = 0;
		for(j;j<1000;j++){	// 计数1000*1000
//			printf("j:%d\n",j);
		}	
	}
}

int main()
{
	struct timeval tvstart;
	struct timeval tvend;
	
	gettimeofday(&tvstart, NULL);	// Get the time since January 1, 1970(获取距1970年1月1日0时0分0秒的时间)
	count();	// 执行函数
	gettimeofday(&tvend, NULL);	// Get the time since January 1, 1970(获取距1970年1月1日0时0分0秒的时间)

	long t_us = 1000000 * (tvend.tv_sec - tvstart.tv_sec) + (tvend.tv_usec - tvstart.tv_usec);	// Calculate microsecond time(计算微秒时间)

	printf("Elapsed timex:%ldus\n",t_us);	// print (打印) 

	return 0;
}
