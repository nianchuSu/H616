#include <stdio.h>
#include <stdlib.h>
#include "retfunc.h"

int main()
{
	char *str=NULL;

	py_init();

	str = par_retfunc(str);
	printf("str:%s\n",str);

	py_finish();
	
	if(str){	// 判断是否为空,不为空释放内存
		free(str);
	}
}