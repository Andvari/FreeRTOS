/*
 * header.h
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#ifndef HEADER_H_
#define HEADER_H_

void vInitTask(void *);
void vConsoleTask(void *);
void vPrintTask(void *);

void NVIC_CFG(void);
void USART1_CFG(void);
void GPIOC_CFG(void);
void RCC_CFG(void);
void USART1_SendString(char *);


#endif /* HEADER_H_ */
