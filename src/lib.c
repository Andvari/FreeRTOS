/*
 * lib.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "./include/lib.h"

void print(char *str){
	int i;
	xSemaphoreTake(sem[SEM_PRINT_QUEUE_SYNC], portMAX_DELAY);

	if((idx_wr_print_queue+1)%MAX_PRINT_QUEUE_SIZE != idx_rd_print_queue){
		i=0;
		do{
			print_queue[idx_wr_print_queue][i] = str[i];
		}while(str[i++]!=0);

		do{
		}while(xQueueSend(queue_print_sync, NULL, portMAX_DELAY) != pdTRUE);

		idx_wr_print_queue = (idx_wr_print_queue+1)%MAX_PRINT_QUEUE_SIZE;
	}

	xSemaphoreGive(sem[SEM_PRINT_QUEUE_SYNC]);
}

int strlen(char *str){
	int i;

	i=0;
	do{
	}while(str[i++]!=0);

	return (i-1);
}

int strcmp(char *str1, char *str2){
	int i;

	if(strlen(str1) != strlen(str2)) return (0);

	i=0;
	do{
		if(str1[i] != str2[i]) return (0);
	}while(str1[i++] != 0);

	return (1);
}

int itoa(int val, char *dst){
	int i=0, j;
	int tmp;
	char a;

	if(val == 0){
		dst[0] = '0';
		dst[1] = 0;
		return (1);
	}

	if(val<0)	tmp = -val;
	else		tmp =  val;

	while(tmp > 0){
		a = tmp%10 + 48;
		dst[i++] = a;
		tmp /= 10;
	}

	if( val < 0 )dst[i++] = '-';

	dst[i] = 0;

	for(j=0; j<i/2; j++){
		a = dst[j];
		dst[j] = dst[i-j-1];
		dst[i-j-1] = a;
	}

	return (i);
}

int ftoa(float val, char *dst){
	int b;
	int sz;

	b = (val - (int)val)*100000;
	if(b<0) b = -b;

	sz = itoa((int)val, dst);
	dst[sz++] = '.';

	if(b<10000)	dst[sz++] = '0';
	if(b<1000)	dst[sz++] = '0';
	if(b<100)	dst[sz++] = '0';
	if(b<10)	dst[sz++] = '0';

	return (sz + itoa(b, &dst[sz]));
}
