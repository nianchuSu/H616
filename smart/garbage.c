#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <pthread.h>
#include <errno.h>

#include "uarttool.h"
#include "retfunc.h"
#include "pwm_sg90.h"

static int detect_process(const char *process_name) // 判断mjpg服务是否在运行
{
    int n = -1;
    FILE *strm;
    char buf[128] = {0};
    sprintf(buf, "ps -ax | grep %s|grep -v grep", process_name);
    if ((strm = popen(buf, "r")) != NULL)
    {
        if (fgets(buf, sizeof(buf), strm) != NULL)
        {
            n = atoi(buf);
        }
    }
    else
    {
        return -1;
    }
    pclose(strm);
    return n;
}


int uart_fd = -1;      // 串口定义
pthread_cond_t cond;   // 线程调节变量
pthread_mutex_t mutex; // 线程锁

// 语音模块
void *voice_void(void *arg) // 线程函数 线程要做什么事情功能
{
    unsigned char buf[6] = {0xAA, 0x55, 0x00, 0x00, 0x55, 0xAA};
    int len = 0;
    if (-1 == uart_fd)
    {
        // __FILE__, __func__, __LINE__     分别是文件名,函数,行号
        printf("%s%s%d\n", __FILE__, __func__, __LINE__);
        pthread_exit(0); // 线程退出?
    }

    while (1)
    {
        len = uart_Getstr(uart_fd, buf, 6); // 获取语音模块的输入

        if (len > 0 && buf[2] == 0x46)
        {
            pthread_mutex_lock(&mutex); // 线程上锁
            buf[2] = 0x00;
            pthread_cond_signal(&cond);   // 发信号
            pthread_mutex_unlock(&mutex); // 解锁
        }
    }
    pthread_exit(0);
}

// 开盖
void *uncap_void(void *arg)
{
    pthread_detach(pthread_self());            // 线程分离,自己释放资源,不需要父线程等待
    unsigned char *buf = (unsigned char *)arg; // 强转数据类型
    if (buf[2] == 0x43)
    {
        open_sg(P5); // 开盖
        delay(2000); // 持续时间
        stop_sg(P5); // 关盖
    }
    // else if (buf[2] == 0x41 || buf[2] == 0x42 || buf[2] == 0x44)
    else if (buf[2] != 0x45)
    {
        open_sg(P7); // 开盖
        delay(2000); // 持续时间
        stop_sg(P7); // 关盖
    }
    pthread_exit(0);
}

// 语音播报
void *report_void(void *arg)
{
    pthread_detach(pthread_self());
    unsigned char *buf = (unsigned char *)arg; // 强转数据类型
    if (-1 == uart_fd)
    {
        // __FILE__, __func__, __LINE__     分别是文件名,函数,行号
        printf("%s%s%d\n", __FILE__, __func__, __LINE__);
        pthread_exit(0); // 线程退出?
    }
    if (NULL != buf)
    {
        uart_Puts(uart_fd, buf, 6); // 发送数据给语音模块
    }

    pthread_exit(0);
}

// 垃圾分类
void *gar_void(void *arg) // 线程函数 线程要做什么事情功能
{
    unsigned char buf[6] = {0xAA, 0x55, 0x00, 0x00, 0x55, 0xAA};
    char *gar_str = NULL;
    pthread_t uncap_tid, report_tid; // 线程id
    while (1)
    {
        pthread_mutex_lock(&mutex);       // 线程上锁/拿锁
        pthread_cond_wait(&cond, &mutex); // 等待信号/接收信号?
        pthread_mutex_unlock(&mutex);     // 解锁
		
		buf[2] = 0x00; // 初始化
        system(SHOOT);                       // 调用摄像头
        if (0 == access(GARBAGE_FILE, F_OK)) // 判断图像是否存在
        {
            gar_str = par_retfunc(gar_str); // 调用python代码,获得返回值
            if (strstr(gar_str, "干垃圾"))
            {
                buf[2] = 0x41;
            }
            else if (strstr(gar_str, "湿垃圾"))
            {
                buf[2] = 0x42;
            }
            else if (strstr(gar_str, "可回收垃圾"))
            {
                buf[2] = 0x43;
            }
            else if (strstr(gar_str, "有害垃圾"))
            {
                buf[2] = 0x44; // buf写入具体数据
            }
            else // 识别失败
            {
                buf[2] = 0x45; // buf写入具体数据
            }
        }
        else
        {
            buf[2] = 0x45; // 没有图像
        }

        // 垃圾桶开盖
        pthread_create(&uncap_tid, NULL, uncap_void, (void *)buf);

        // 语音播报
        pthread_create(&report_tid, NULL, report_void, (void *)buf);
        printf("buf[2]--%c\n",buf[2]);
        // buf[2] = 0x00; // 发送完成后初始化buf
        remove(GARBAGE_FILE);
        printf("buf[2]--%c\n",buf[2]);
    }
    pthread_exit(0); // 线程退出?
}

int main(int argc, char **argv)
{
    int ret = -1;

    pthread_t voice_tid, gar_tid; // 线程id

    wiringPiSetup(); // 初始化香橙派
    // 初始化阿里云Pythong接口
    py_init();

    system("sudo /home/orangepi/mjpg.sh &");    // 启动USB摄像头服务
    ret = detect_process("mjpg"); // 用于判断mjpg服务是否已经启动

    	
        
       	if (ret < 0)
			goto END;

    printf("服务启动成功\n");
	printf("程序运行正常ret:%d\n",ret);
    
    // if (-1 == ret)
    // {
    //     goto END;
    // }
    // 初始化香橙派串口
    uart_fd = uart_Open(URAT, BAUD);

    // 判断串口是否初始化成功
    if (-1 == uart_fd)
    {
        goto END; // 失败直接跳转释放Python解释器 关闭串口初始化 结束程序
    }

    // 语音线程
    pthread_create(&voice_tid, NULL, voice_void, NULL);

    // 垃圾分类线程
    pthread_create(&gar_tid, NULL, gar_void, NULL);

    pthread_join(voice_tid, NULL); // 等待线程结束
    pthread_join(gar_tid, NULL);

    pthread_mutex_destroy(&mutex); // 释放锁    // 解锁?
    pthread_cond_destroy(&cond);

    close(uart_fd); // 关闭串口

END:
    py_finish(); // 释放python解释器

    return 0;
}
