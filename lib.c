/*
 * lib.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "globalvars.h"

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


