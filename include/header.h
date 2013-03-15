/*
 * header.h
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#ifndef HEADER_H_
#define HEADER_H_

#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "defines.h"
#include "globalvars.h"
#include "lib.h"

void vInitTask(void *);
void vConsoleTask(void *);
void vPrintTask(void *);

void NVIC_CFG(void);
void USART1_CFG(void);
void GPIOC_CFG(void);
void RCC_CFG(void);

#endif /* HEADER_H_ */
