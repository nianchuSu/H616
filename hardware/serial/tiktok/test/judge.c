#include <stdio.h>
#include <string.h>

int main()
{
	FILE *file;
	char buf[128] = {'\0'};

	char *cmd = "ps -elf|grep tiktok |grep -v grep";

	file = popen(cmd,"r");

	fgets(buf,128,file);
	if(strstr(buf,"tiktok") !=	NULL){
		printf("tiktok is running.\n");
	}else{
		printf("tiktok is not running.\n");
	}
	
	printf("buf:%s\n",buf);
	return 0;
}
