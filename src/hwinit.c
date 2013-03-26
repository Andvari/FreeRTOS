/*
 * hwinit.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "header.h"

void RCC_CFG(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
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

  NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void I2C2_CFG(void){
	I2C_InitTypeDef myI2C;
	GPIO_InitTypeDef myGPIO;

	GPIO_StructInit(&myGPIO);

	myGPIO.GPIO_Speed	=	GPIO_Speed_10MHz;
	myGPIO.GPIO_Pin		=	GPIO_Pin_10;
	myGPIO.GPIO_Mode	=	GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &myGPIO);
	myGPIO.GPIO_Pin		=	GPIO_Pin_11;
	myGPIO.GPIO_Mode	=	GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &myGPIO);

	I2C_StructInit(&myI2C);

	myI2C.I2C_ClockSpeed			=	100000;
	myI2C.I2C_Mode					=	I2C_Mode_I2C;
	myI2C.I2C_DutyCycle				=	I2C_DutyCycle_2;
	myI2C.I2C_OwnAddress1			=	0x0;
	myI2C.I2C_Ack					=	I2C_Ack_Enable;
	myI2C.I2C_AcknowledgedAddress	=	I2C_AcknowledgedAddress_7bit;

	I2C_Init(BMP085_I2C, &myI2C);
	I2C_Cmd(BMP085_I2C, ENABLE);

	I2C_ClearITPendingBit(BMP085_I2C,I2C_IT_TIMEOUT);
	I2C_ClearITPendingBit(BMP085_I2C,I2C_IT_OVR);
	I2C_ClearITPendingBit(BMP085_I2C,I2C_IT_AF);
	I2C_ClearITPendingBit(BMP085_I2C,I2C_IT_ARLO);
	I2C_ClearITPendingBit(BMP085_I2C,I2C_IT_BERR);
	I2C_ClearITPendingBit(BMP085_I2C,I2C_IT_STOPF);
	I2C_ClearITPendingBit(BMP085_I2C,I2C_IT_ADD10);
	I2C_ClearITPendingBit(BMP085_I2C,I2C_IT_BTF);
	I2C_ClearITPendingBit(BMP085_I2C,I2C_IT_ADDR);
	I2C_ClearITPendingBit(BMP085_I2C,I2C_IT_SB);

	//I2C_ITConfig(BMP085_I2C, I2C_IT_EVT, ENABLE);
	I2C_ITConfig(BMP085_I2C, I2C_IT_BUF, ENABLE);

	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 0);
	vTaskDelay(2);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1);
}

