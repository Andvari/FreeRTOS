/*
 * irqhandlersinit.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "header.h"

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

extern int sr1;
extern int sr2;
extern int i2c2_stage;
extern int ev8_stage;
extern unsigned char i2c2_addr;
extern unsigned char msb;
extern unsigned char lsb;
extern int aaa;
extern int bbb;
extern int ccc;
extern int ddd;
extern int eee;
extern int fff;

#define	EV5		((I2C2->SR1&0x1) != 0)
#define EV6		((I2C2->SR1&0x2) != 0)
#define EV8		((I2C2->SR1&0x80) != 0)

int arr_sr1[20];
int arr_sr2[20];
int ct_arr;

void I2C2_EV_IRQHandler(void){
	aaa++;
}

