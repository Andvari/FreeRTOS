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

void vMyTask(void *);
void NVIC_CFG(void);
void USART1_CFG(void);
void GPIOC_CFG(void);
void RCC_CFG(void);

int bit = 0;
char symbol;
int main(void){
	int i, j, k;
	portBASE_TYPE code=pdPASS+1;


	RCC_CFG();

//	USART1_CFG();

	GPIOC_CFG();

	NVIC_CFG();

	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1);

	code = pdFAIL;
	xTaskCreate(&vMyTask, (signed char *)"My Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	if(code == pdPASS){
		for(i=0; i<10; i++){
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, ++bit%2);

			for(j=0; j<1000000; j++){
				k++;
			}
		}
	}

	vTaskStartScheduler();

	for(i=0; i<20; i++){
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, bit);
		bit = 1 - bit;

		for(j=0; j<100000; j++){
			k++;
		}
	}

	for(;;);
	return 0;
}


int a;
void vMyTask(void *pvParameters){
	USART1_CFG();
	a=0;
	for(;;){
		if(a!=0){
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, ++bit%2);
			a=0;
			taskYIELD();
		}
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
/*
void USART_SendString(char *str){
	int i;

	for(i=0; i<strlen(str); i++){
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
		USART_SendData(USART1, str[i]);
	}

}
*/
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
	//GPIO_WriteBit(GPIOC, GPIO_Pin_13, ++bit%2);
    a=1;
  }
}
