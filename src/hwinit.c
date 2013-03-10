/*
 * hwinit.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "header.h"

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

	myUSART.USART_BaudRate				=	115200;
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

