/*
 * main.c
 *
 *  Created on: Mar 2, 2013
 *      Author: nemo
 */

#include "header.h"

void vTimerTask(void *);

int main(void){
	int i;

	RCC_CFG();

	for(i=0 ;i<sizeof(sem); i++){
		vSemaphoreCreateBinary(sem[i]);
		xSemaphoreTake(sem[i], portMAX_DELAY);
	}

	for(i=0; i<MAX_NUM_VARS; i++){
		vars[i] = 0;
	}

	queue_print_sync = xQueueCreate(MAX_PRINT_QUEUE_SIZE, 0);

	xSemaphoreGive(sem[SEM_PRINT_QUEUE_SYNC]);
	idx_rd_cmd_queue	=	0;
	idx_wr_cmd_queue	=	0;
	len_wr_cmd_queue	=	0;

	idx_rd_print_queue	=	0;
	idx_wr_print_queue	=	0;

	xTaskCreate(vInitTask,		(signed char *)"Init",		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vConsoleTask,	(signed char *)"Console",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vPrintTask,		(signed char *)"Print",		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vTimerTask,		(signed char *)"Timer6",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

	vTaskStartScheduler();

	for(;;);
	return (0);
}

void vTimerTask(void *pvParameters){

	timer6_counter = 0;

	for(;;){
		xSemaphoreTake(sem[SEM_TIM2_IT_Update], portMAX_DELAY);
		//print("\r\ntimer6: %d %d", SystemCoreClock, timer6_counter++);
		print("\r\ntimer6: %x", RTC_GetCounter());
	}
}
