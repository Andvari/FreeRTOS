/*
 * console.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "header.h"

void vConsoleTask(void *pvParameters){
	int i;
	char command[16];
	char arg1[16];
	char arg2[16];
	char *remain_str;
	char *address = NULL;
	unsigned int val = 256;
	char num_args;


	USART1_CFG();

	for(i=0; i<12; i++){
		print("\n\n\n\n\n\n\n\n");
		vTaskDelay(1);
	}

	print("\r\nSTM32F10x monitor [ver 1.0]");

	for(;;){
		print("\r\n->");
		xSemaphoreTake(sem[SEM_USART1_RXNE], portMAX_DELAY);

		command[0]	=	0;
		arg1[0]		=	0;
		arg2[0]		=	0;
		remain_str = get_token(&cmd_queue[idx_rd_cmd_queue][0],		command,	' ');num_args=0;
		if(remain_str != NULL){remain_str = get_token(remain_str,	arg1,		' ');num_args=1;}
		if(remain_str != NULL){remain_str = get_token(remain_str,	arg2,		' ');num_args=2;}

		idx_rd_cmd_queue = (idx_rd_cmd_queue+1)%MAX_CMD_QUEUE_SIZE;

		if(strcmp(command, "hs") == 0){
			print("\r\nHeap size: %d [%x]", xPortGetFreeHeapSize(), xPortGetFreeHeapSize());
			continue;
		}

		if(strcmp(command, "di") == 0){

			if(num_args > 0){
				address	=	(char *)atoi(arg1);
			}

			if(num_args > 1){
				if((val = atoi(arg2)) > 1024) val = 1024;
			}

			dump(address, val);

			address	+=	val&0xFFFFFFF0;

			if(val%0x10 != 0) address	+=	0x10;

			continue;
		}

		if(strcmp(command, "mi") == 0){
			address = NULL;
			val = 0;

			address = (char *)atoi(arg1);
			val = atoi(arg2)&0xFF;

			if(((int)address >= SRAM_START_ADDRESS) && ((int)address < SRAM_START_ADDRESS + SRAM_SIZE)){
				address[0] = (char)val;

				if(address[0] == (char)val){
					print("\r\nOK");
				}
				else{
					print("\r\nError");
				}
			}

			continue;
		}

		if(strcmp(command, "reset") == 0){
			NVIC_SystemReset();
		}

	}
}

