/*
 * main.c
 *
 *  Created on: Mar 2, 2013
 *      Author: nemo
 */

#include "header.h"

void vI2CTask(void *);
void vMonitorTask(void *);


union{
	short int	si;
	struct{
		char 	MSB;
		char	LSB;
	}byte;
}AC1;

short int AC2;
short int AC3;
short int AC4;
short int AC5;
short int AC6;
short int B1;
short int B2;
short int MB;
short int MC;
short int MD;


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
	xTaskCreate(vI2CTask,		(signed char *)"I2C",		configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vMonitorTask,	(signed char *)"Monitor",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

	vTaskStartScheduler();

	for(;;);
	return (0);
}


void vI2CTask(void *pvParameters){

	I2C_InitTypeDef myI2C;
	GPIO_InitTypeDef myGPIO;


	vTaskDelay(400);

	GPIO_StructInit(&myGPIO);
/*
	myGPIO.GPIO_Pin		=	GPIO_Pin_10 | GPIO_Pin_11;
	myGPIO.GPIO_Mode	=	GPIO_Mode_AF_PP;
	myGPIO.GPIO_Speed	=	GPIO_Speed_50MHz;
*/

	myGPIO.GPIO_Speed	=	GPIO_Speed_10MHz;
	myGPIO.GPIO_Pin		=	GPIO_Pin_10;
	myGPIO.GPIO_Mode	=	GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &myGPIO);
	myGPIO.GPIO_Pin		=	GPIO_Pin_11;
	myGPIO.GPIO_Mode	=	GPIO_Mode_AF_OD;

	GPIO_Init(GPIOB, &myGPIO);


	I2C_StructInit(&myI2C);

	  myI2C.I2C_ClockSpeed			=	100000;
	  myI2C.I2C_Mode				=	I2C_Mode_I2C;
	  myI2C.I2C_DutyCycle			=	I2C_DutyCycle_2;
	  myI2C.I2C_OwnAddress1			=	0x0;
	  myI2C.I2C_Ack					=	I2C_Ack_Enable;
	  myI2C.I2C_AcknowledgedAddress	=	I2C_AcknowledgedAddress_7bit;

	I2C_Init(I2C2, &myI2C);

	I2C_Cmd(I2C2, ENABLE);


	//I2C_GenerateSTART(I2C1, ENABLE);
	//I2C_Send7bitAddress(I2C1, 0x77, I2C_Direction_Receiver);
	//A1 = I2C_ReceiveData(I2C1);
	//A1 = A1 << 8 | I2C_ReceiveData(I2C1);

	//print("\r\n%x", A1);




	print("\r\n%d %d", I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED), SUCCESS);
	for(;;){
		I2C_GenerateSTART(I2C2, ENABLE);
		while(!I2C_GetFlagStatus(I2C2, I2C_FLAG_SB));

		I2C_Send7bitAddress(I2C2, 0xEE, I2C_Direction_Transmitter);
		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

		I2C_SendData(I2C2, 0xAA);
		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		I2C_GenerateSTOP(I2C2, ENABLE);
		while(I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF));

		I2C_GenerateSTART(I2C2, ENABLE);
		while(!I2C_GetFlagStatus(I2C2, I2C_FLAG_SB));

		I2C_Send7bitAddress(I2C2, 0xEE, I2C_Direction_Receiver);
		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));


		AC1.byte.MSB = I2C_ReceiveData(I2C2);
		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));

		AC1.byte.LSB = I2C_ReceiveData(I2C2);
		while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));

		I2C_GenerateSTOP(I2C2, ENABLE);
		while(I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF));

		//print("\r\n%x", A1);

		vTaskDelay(1);
	}
}

void vMonitorTask(void *pvParameters){

	vTaskDelay(400);

	for(;;){
		print("\r\nAC1= %d %d", AC1.byte.MSB, AC1.byte.LSB);
		vTaskDelay(200);
	}
}
