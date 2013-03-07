/*
 * printspool.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"


#include "FreeRTOS.h"
#include "semphr.h"

#include "./include/defines.h"
#include "./include/globalvars.h"

extern void USART1_SendString(char *);

void vPrintTask(void *pvParameters){

	for(;;){
		xQueueReceive(queue_print_sync, NULL, portMAX_DELAY);
		USART1_SendString(&print_queue[idx_rd_print_queue][0]);
		idx_rd_print_queue = (idx_rd_print_queue+1)%MAX_PRINT_QUEUE_SIZE;
	}

}

