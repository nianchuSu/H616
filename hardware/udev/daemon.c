#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

static bool flag = true;

void handler(int sig)
{
	printf("signal handler:%d\n",sig);
	flag = false;
}

int judge()
{
	FILE *file;
	char buf[128] = {'\0'};

	char *cmd = "ps -elf|grep tiktok |grep -v grep";

	file = popen(cmd,"r");

	fgets(buf,128,file);
	if(strstr(buf,"tiktok") !=	NULL){
		return 0;
	}else{
		return -1;
	}
}

int main()
{
	time_t t;
	int fd;
	if(daemon(0,0) == -1){	// Creating a daemon 
		perror("daemon");
		exit(-1);
	}

	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(sigaction(SIGQUIT,&act,NULL)){
		printf("signal successful.");
		exit(0);
	}

	while(flag){
		if(judge() == -1){
			system("/home/orangepi/hardware/serial/tiktok/test/tiktok /dev/ttyS5 &");	// &后台运行
		}
		sleep(3);	// 3s
	}

	return 0;
}
