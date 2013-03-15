/*
 * init.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "header.h"

void vInitTask(void *pvParameters){

	NVIC_CFG();

	GPIOC_CFG();

	TIM2_CFG();

	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1);

	vTaskSuspend(NULL);

}


