/*
 * console.c
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

extern void USART1_CFG(void);
extern void print(char *);

void vConsoleTask(void *pvParameters){

	USART1_CFG();

	print("\r\nSTM32F10x monitor [ver 1.0]");

	for(;;){
		print("\r\n->");
		xSemaphoreTake(sem[SEM_USART1_RXNE], portMAX_DELAY);

		print("\r\n");
		print(&cmd_queue[idx_rd_cmd_queue][0]);
		idx_rd_cmd_queue = (idx_rd_cmd_queue+1)%MAX_CMD_QUEUE_SIZE;
	}
}
