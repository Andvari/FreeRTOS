/*
 * console.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "header.h"

void vConsoleTask(void *pvParameters){
	int i;

	USART1_CFG();

	for(i=0; i<12; i++){
		print("\n\n\n\n\n\n\n\n");
		vTaskDelay(1);
	}

	print("\r\nSTM32F10x monitor [ver 1.0]");

	for(;;){
		print("\r\n->");
		xSemaphoreTake(sem[SEM_USART1_RXNE], portMAX_DELAY);

		if(strcmp(&cmd_queue[idx_rd_cmd_queue][0], "hs") == 0){
			print("\r\nHeap size: %d", xPortGetFreeHeapSize());
		}
		else{
			if(strlen(&cmd_queue[idx_rd_cmd_queue][0])){
				print("\r\n");
				print(&cmd_queue[idx_rd_cmd_queue][0]);
			}
		}
		idx_rd_cmd_queue = (idx_rd_cmd_queue+1)%MAX_CMD_QUEUE_SIZE;
	}
}

