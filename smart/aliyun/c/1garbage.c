#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

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

int main(int argc, char **argv)
{
    int uart_fd = -1; // 串口定义
                      // 定义buf并初始化数据
    unsigned char buf[6] = {0xAA, 0x55, 0x00, 0x00, 0x55, 0xAA};
    // 定义
    int len = 0;
    int ret = -1;
    // 阿里云接口指针
    char *gar_str = NULL;

    wiringPiSetup();    // 初始化香橙派
    // 初始化阿里云Pythong接口
    py_init();

    ret = detect_process("mjpg"); // 用于判断mjpg服务是否已经启动
    if ( -1 == ret) 
    {
         goto END;
    }
    // 初始化香橙派串口
    uart_fd = uart_Open(URAT, BAUD);

    // 判断串口是否初始化成功
    if (-1 == uart_fd)
    {
        goto END; // 失败直接跳转释放Python解释器 关闭串口初始化 结束程序
    }
    // printf("%c\n",buf[2]);
    // 香橙派获取语音模块输入
    while (1)
    {
        len = uart_Getstr(uart_fd, buf, 6); // 获取语音模块的输入
        // printf("%d\n",len);
        if (len > 0 && buf[2] == 0x46)
        {

            buf[2] = 0x00;                       // 初始化buf数据
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
            // printf("%c\n",buf[2]);
            uart_Puts(uart_fd, buf, 6); // 发送数据给语音模块
            
            if (buf[2] == 0x43)
            {
                open_sg(P5);    // 开盖
                delay(2000);    // 持续时间
                stop_sg(P5);    // 关盖
            }
            
            buf[2] = 0x00;              // 发送完成后初始化buf
            remove(GARBAGE_FILE);       // 删除图像
        }
        if (len > 0 && buf[2] == 0x47)     // 这个位置判断
        {
            buf[2] = 0x00;
            goto END;
        }  
        
    }
    close(uart_fd);     // 关闭串口

END:
    py_finish(); // 释放python解释器

    return 0;
}