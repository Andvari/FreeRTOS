/*
 * init.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "FreeRTOS.h"
#include "task.h"

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

extern void NVIC_CFG(void);
extern void GPIOC_CFG(void);

void vInitTask(void *pvParameters){

	NVIC_CFG();

	GPIOC_CFG();

	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1);

	vTaskSuspend(NULL);

}


