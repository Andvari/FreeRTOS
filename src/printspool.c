/*
 * printspool.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "header.h"

void vPrintTask(void *pvParameters){
	int i;

	for(;;){
		xQueueReceive(queue_print_sync, NULL, portMAX_DELAY);
		for(i=0; print_queue[idx_rd_print_queue][i]!=0; i++){
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
			USART_SendData(USART1, print_queue[idx_rd_print_queue][i]);
		}
		idx_rd_print_queue = (idx_rd_print_queue+1)%MAX_PRINT_QUEUE_SIZE;
	}

}
