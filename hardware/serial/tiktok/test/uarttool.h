int uart_Open (const char *device, const int baud);	// 重写打开

void uart_Puts (const int fd, const char *s);	// 发送字符串

int uart_Getstr (const int fd,char *buf);	// 写入?

char uart_getc();
