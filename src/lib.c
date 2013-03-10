/*
 * lib.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "./include/lib.h"

#define	MAX_LINE_SIZE	80

void print(char *format, ...){
	int i;
	int len;
	int state;
	char line[MAX_LINE_SIZE];
	char item[16];
	va_list args;

	i		=	0;
	len		=	0;
	state	=	0;

	va_start(args, *format);
	do{
		if(format[i] == '%'){
			if(state == 0){
				state = 1;
			}
			else{
				state = 0;
				line[len++] = format[i];
			}
		}
		else{
			if(state == 0){
				line[len++] = format[i];
			}
			else{
				state = 0;
				switch(format[i]){
								case	'd':	strcpy(&line[len], itoa(va_arg(args, int), item, 10));
												break;
								case	'f':	strcpy(&line[len], ftoa(va_arg(args, double), item));
												break;
								case	'x':	strcpy(&line[len], itoa(va_arg(args, int), item, 16));
												break;
								case	'c':	line[len++] = va_arg(args, int);
												line[len] = 0;
												break;
								case	's':	strcpy(&line[len], va_arg(args, char *));
												break;
								default:
												break;
				}
				len = strlen(line);
			}
		}
	}while(format[i++]!=0);

	va_end(args);

	xSemaphoreTake(sem[SEM_PRINT_QUEUE_SYNC], portMAX_DELAY);
	if((idx_wr_print_queue+1)%MAX_PRINT_QUEUE_SIZE != idx_rd_print_queue){
		i=0;
		do{
			print_queue[idx_wr_print_queue][i] = line[i];
		}while(line[i++]!=0);

		do{
		}while(xQueueSend(queue_print_sync, NULL, portMAX_DELAY) != pdTRUE);

		idx_wr_print_queue = (idx_wr_print_queue+1)%MAX_PRINT_QUEUE_SIZE;
	}

	xSemaphoreGive(sem[SEM_PRINT_QUEUE_SYNC]);
}

char *itoa(int val, char *dst, int radix){
	int i=0, j;
	unsigned int tmp;
	char a;

	if(val == 0){
		dst[0] = '0';
		dst[1] = 0;
	}
	else{

		tmp = val;
		if((val<0)&&(radix==10)) tmp = -val;

		while(tmp > 0){
			a = tmp%radix + 48;
			if(a>57)a+=7;
			dst[i++] = a;
			tmp /= radix;
		}

		if(radix == 16){
			while(i<8){
				dst[i++] = '0';
			}
			dst[i++] = 'x';
			dst[i++] = '0';
		}
		else{
			if( val < 0 )dst[i++] = '-';
		}

		dst[i] = 0;

		for(j=0; j<i/2; j++){
			a = dst[j];
			dst[j] = dst[i-j-1];
			dst[i-j-1] = a;
		}
	}

	return (dst);
}

char *ftoa(float val, char *dst){
	int b;
	int sz;

	b = (val - (int)val)*100000;
	if(b<0) b = -b;

	itoa((int)val, dst, 10);
	sz = strlen(dst);
	dst[sz++] = '.';

	if(b<10000)	dst[sz++] = '0';
	if(b<1000)	dst[sz++] = '0';
	if(b<100)	dst[sz++] = '0';
	if(b<10)	dst[sz++] = '0';

	return (dst);
}

int atoi(char *str){
	int i;
	int val;

	i=0;
	val = 0;
	while((str[i]>=48)&&(str[i]<=57)){
		val = val*10 + str[i++] - 48;
	}

	return (val);
}
