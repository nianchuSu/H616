#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "oled.h"
#include "font.h"

int oled_demo(struct display_info *disp) {
	int i;
	char buf[100];

	oled_putstrto(disp, 0, 4, "OLED Online.");
	oled_putstrto(disp, 0, 48, "DATE:2024.05.07");
	oled_putstrto(disp, 0, 56, "Time:16:57");
	oled_send_buffer(disp);

	disp->font = font3;
	for (i=0; i<128; i++) {
		oled_putstrto(disp, 127-i, 36+4, "I am name nianxing_su. ");
		oled_send_buffer(disp);

		if(i == 127){
			int j = 0;
			sleep(1);
			while(1){
				oled_putstrto(disp, 0+j, 36+4, "                                 ");
				oled_send_buffer(disp);
				if(j == i){
		   			i = 0;
					break;
				}
				j++;
			}
		}
	}
return 0;
}

void show_error(int err, int add) {
	//const gchar* errmsg;
	//errmsg = g_strerror(errno);
	printf("\nERROR: %i, %i\n\n", err, add);
	//printf("\nERROR\n");
}

void show_usage(char *progname) {
	printf("\nUsage:\n%s <I2C bus device node >\n", progname);
}

int main(int argc, char **argv) {
	int e;
	char filename[32];
	struct display_info disp;

	if (argc < 2) {
		show_usage(argv[0]);
		
		return -1;
	}

	memset(&disp, 0, sizeof(disp));
	sprintf(filename, "%s", argv[1]);
	disp.address = OLED_I2C_ADDR;
	disp.font = font2;

	e = oled_open(&disp, filename);

	if (e < 0) {
		show_error(1, e);
	} else {
		e = oled_init(&disp);
	if (e < 0) {
		show_error(2, e);
	} else {
		printf("---------start--------\n");
		if (oled_demo(&disp) < 0)
			show_error(3, 777);
			printf("----------end---------\n");
		}
	}

	return 0;
}
