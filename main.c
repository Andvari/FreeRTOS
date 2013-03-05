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

int bit = 0;
int main(void){
	int i, j, k;
	portBASE_TYPE code=pdPASS+1;
	GPIO_InitTypeDef myGPIO;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_StructInit(&myGPIO);
	myGPIO.GPIO_Speed	=	GPIO_Speed_50MHz;
	myGPIO.GPIO_Pin		=	GPIO_Pin_13;
	myGPIO.GPIO_Mode	=	GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &myGPIO);

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


void vMyTask(void *pvParameters){
	for(;;){
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, ++bit%2);
		vTaskDelay(200*3);
	}
}

void vApplicationIdleHook( void ){
	while(1){
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, ++bit%2);
		vTaskDelay(portTICK_RATE_MS*1000);
	}
}
