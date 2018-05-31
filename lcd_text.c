#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "lcd.c"

#define GPIO_PATH "/sys/class/gpio"
#define GPIO_LENGTH 10
#define MAX_BUF 64


int readADC(unsigned int pin);

int main(void)
{
//			 RS  E   D0  D1  D2  D3
	char gpio[10] = {46, 47, 26, 23, 44, 45,
//                               D4  D5  D6  D7
                                 68, 69, 67, 66};

	FILE *gpio_fp[10];
	FILE *tmp_fp;

	char tmp_path[255];
	char *tmp;
	char str[10];

	int i;

	for (i = 0; i < GPIO_LENGTH; i++)
	{
		tmp_fp = fopen(GPIO_PATH "/export", "w");
		fprintf(tmp_fp, "%d", gpio[i]);
		fclose(tmp_fp);

		sprintf(tmp_path, GPIO_PATH "/gpio%d/direction", gpio[i]);
		tmp_fp = fopen(tmp_path, "w");
		fprintf(tmp_fp, "out");
		fclose(tmp_fp);

		sprintf(tmp_path, GPIO_PATH "/gpio%d/value", gpio[i]);
		gpio_fp[i] = fopen(tmp_path, "w");
	}

	system("echo BB-ADC > /sys/devices/bone_capemgr.9/slots");

	while(1){
		lcd_init(gpio_fp);

		int adc4 = readADC(4);
		sprintf(str, "%d", adc4);

		tmp = str; 
		lcd_send(FIRST_LINE, CMD_MODE);
		lcd_write("HUMIDITY");
		
		lcd_send(SECOND_LINE, CMD_MODE);
		lcd_write(tmp);
	
		sleep(3);
	}

	tmp_fp = fopen(GPIO_PATH "/unexport", "w");
	for (i = 0; i < GPIO_LENGTH; i++)
	{
		fclose(gpio_fp[i]);
		fprintf(tmp_fp, "%d", gpio[i]);
	}
	fclose(tmp_fp);

	return 0;
}

int readADC(unsigned int pin)
{
	int fd;
	char buf[MAX_BUF];
	char val[4];

	snprintf(buf, sizeof(buf), "/sys/devices/ocp.3/helper.15/AIN%d", pin);
	fd = open(buf, O_RDONLY);

	if(fd<0) {
		perror("ADC - problem opening ADC");
	}

	read(fd, &val, 4);
	close(fd);

	return atoi(val);
}