/*
 * main.c
 *
 *  Created on: Mar 2, 2013
 *      Author: nemo
 */

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "FreeRTOS.h"

#include "task.h"
#include "queue.h"
#include "semphr.h"

void vMyTask(void *);
void NVIC_CFG(void);
void USART1_CFG(void);
void GPIOC_CFG(void);
void RCC_CFG(void);
void USART1_SendString(char *);

#define MAX_CMD_LEN		32
#define MAX_CMD_QUEUE	2
int bit = 0;

char symbol;
char cmd[MAX_CMD_QUEUE][MAX_CMD_LEN];
int idx_rd_cmd=0;
int idx_wr_cmd=0;
int idx_cmd=0;

xSemaphoreHandle Sem;
int main(void){
	int i, j, k;
	idx_rd_cmd=0;
	idx_wr_cmd=0;
	idx_cmd=0;

	RCC_CFG();

	vSemaphoreCreateBinary(Sem);
	xSemaphoreTake(Sem, portMAX_DELAY);

	xTaskCreate(vMyTask, (signed char *)"Init",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1);


	vTaskStartScheduler();

	for(i=0; i<20; i++){
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, ++bit%2);
		for(j=0; j<300000; j++)k++;
	}

	for(;;);
	return 0;
}

void vMyTask(void *pvParameters){

	NVIC_CFG();

	USART1_CFG();

	GPIOC_CFG();

	USART1_SendString("\r\nSTM32F10x monitor [ver 1.0]\r\n->");

	for(;;){
		xSemaphoreTake(Sem, portMAX_DELAY);
		USART1_SendString("\r\nCommand received: ");
		USART1_SendString(&cmd[idx_rd_cmd][0]);
		USART1_SendString("\r\n->");

		idx_rd_cmd = (idx_rd_cmd+1)%MAX_CMD_QUEUE;

		//GPIO_WriteBit(GPIOC, GPIO_Pin_13, ++bit%2);
	}
}

/*
void vApplicationIdleHook( void ){
	while(1){
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, ++bit%2);
		vTaskDelay(portTICK_RATE_MS*1000);
	}
}
*/

void USART1_SendString(char *str){
	int i;

	for(i=0; str[i]!=0; i++){
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
		USART_SendData(USART1, str[i]);
	}

}

void RCC_CFG(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOC, ENABLE);
}

void GPIOC_CFG(void){
	GPIO_InitTypeDef myGPIO;

	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Speed	=	GPIO_Speed_50MHz;
	myGPIO.GPIO_Pin		=	GPIO_Pin_13;
	myGPIO.GPIO_Mode	=	GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &myGPIO);

}

void USART1_CFG(void){
	GPIO_InitTypeDef myGPIO;
	USART_InitTypeDef myUSART;


	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Speed	=	GPIO_Speed_50MHz;

	myGPIO.GPIO_Pin		=	GPIO_Pin_9;
	myGPIO.GPIO_Mode	=	GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &myGPIO);

	myGPIO.GPIO_Pin		=	GPIO_Pin_10;
	myGPIO.GPIO_Mode	=	GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &myGPIO);

	myUSART.USART_BaudRate				=	38400;
	myUSART.USART_WordLength			=	USART_WordLength_8b;
	myUSART.USART_StopBits				=	USART_StopBits_1;
	myUSART.USART_Parity				=	USART_Parity_No;
	myUSART.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;
	myUSART.USART_Mode					=	USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &myUSART);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
}

void NVIC_CFG(void){

  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler(void){
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
    symbol = USART_ReceiveData(USART1);
	if(symbol == 0x0D){
		cmd[idx_wr_cmd][idx_cmd] = 0;
		idx_cmd = 0;
		idx_wr_cmd = (idx_wr_cmd+1)%MAX_CMD_QUEUE;
		xSemaphoreGive(Sem);
	}
	else{
		if(idx_cmd < MAX_CMD_LEN-1){
			cmd[idx_wr_cmd][idx_cmd++] = symbol;
			USART_SendData(USART1, symbol);
		}
	}
  }
}
