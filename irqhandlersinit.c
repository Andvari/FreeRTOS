/*
 * irqhandlersinit.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "globalvars.h"

void USART1_IRQHandler(void){
  char symbol;

  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
    symbol = USART_ReceiveData(USART1);
	if(symbol == 0x0D){
		cmd_queue[idx_wr_cmd_queue][idx_cmd_queue] = 0;
		idx_cmd_queue = 0;
		idx_wr_cmd_queue = (idx_wr_cmd_queue+1)%MAX_CMD_QUEUE_SIZE;
		xSemaphoreGiveFromISR(sem[SEM_USART1_RXNE], pdFALSE);
	}
	else{
		if(idx_cmd_queue < MAX_CMD_QUEUE_RECORD_LEN-1){
			cmd_queue[idx_wr_cmd_queue][idx_cmd_queue++] = symbol;
			USART_SendData(USART1, symbol);
		}
	}
  }
}

