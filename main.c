/*
 * main.c
 *
 *  Created on: Mar 2, 2013
 *      Author: nemo
 */

#include "header.h"

void vI2CTask(void *);
void vTempMeasTask(void *);
void vPressMeasTask(void *);

#define	BASE_NUM_REGS	11
#define	NUM_REGS		13
#define	AC1			0
#define	AC2			1
#define	AC3			2
#define	AC4			3
#define	AC5			4
#define	AC6			5
#define	B1			6
#define	B2			7
#define	MB			8
#define	MC			9
#define	MD			10
#define	UT			11
#define	UP			12

union{
	signed short int	ssi;
	unsigned short int	usi;
	struct{
		unsigned char 	LSB;
		unsigned char	MSB;
	}byte;
}regs[NUM_REGS];


int sr1;
int sr2;
int i2c2_stage;
unsigned char i2c2_addr;
unsigned char msb;
unsigned char lsb;

int aaa;
int bbb;
int ccc;
int ddd;
int eee;
int fff;

int main(void){
	int i;

	RCC_CFG();

	sr1=0;
	sr2=0;

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
	xTaskCreate(vTempMeasTask,	(signed char *)"Temperature", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vPressMeasTask,	(signed char *)"Pressure",	configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

	vTaskStartScheduler();

	for(;;);
	return (0);
}


void vI2CTask(void *pvParameters){
	int i;

	vTaskDelay(200);

	i2c2_addr  = 0xAA;
	for(i=0; i<BASE_NUM_REGS; i++){
		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->DR = 0xEE;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		aaa = I2C2->SR2;
		while((I2C2->SR1&0x80) == 0);				// TXE
		I2C2->DR = i2c2_addr;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->CR1 |= 0x400;							//ACK
		I2C2->DR = 0xEF;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		aaa = I2C2->SR2;
		while((I2C2->SR1&0x40) == 0);				// RXNE
		regs[i].byte.MSB = I2C2->DR;
		while((I2C2->SR1&0x40) == 0);				// RXNE
		I2C2->CR1 &= 0xFFFFFBFF;					// NACK
		I2C2->CR1 |= 0x200;
		regs[i].byte.LSB = I2C2->DR;

		i2c2_addr += 2;
	}

/*
	print("\r\nAC1= %d  ",	regs[AC1].ssi);
	print("AC2= %d  ",		regs[AC2].ssi);
	print("AC3= %d  ",		regs[AC3].ssi);
	print("AC4= %d  ",		regs[AC4].usi);
	print("AC5= %d  ",		regs[AC5].usi);
	print("AC6= %d  ",		regs[AC6].usi);
	print("B1= %d  ",		regs[B1].ssi);
	print("B2= %d  ",		regs[B2].ssi);
	print("MB= %d  ",		regs[MB].ssi);
	print("MC= %d  ",		regs[MC].ssi);
	print("MD= %d  ",		regs[MD].ssi);
*/
	xSemaphoreGive(sem[SEM_TO_MEAS]);

	vTaskSuspend(NULL);
}

signed long T;
signed long X1;
signed long X2;
signed long B5;
void vTempMeasTask(void *pvParameters){


	for(;;){
		vTaskDelay(200);
		xSemaphoreTake(sem[SEM_TO_MEAS], portMAX_DELAY);

		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->DR = 0xEE;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		aaa = I2C2->SR2;
		while((I2C2->SR1&0x80) == 0);				// TXE
		I2C2->DR = 0xF4;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->DR = 0x2E;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->CR1 |= 0x200;
		vTaskDelay(1);

		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->DR = 0xEE;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		aaa = I2C2->SR2;
		while((I2C2->SR1&0x80) == 0);				// TXE
		I2C2->DR = 0xF6;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->CR1 |= 0x400;							//ACK
		I2C2->DR = 0xEF;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		aaa = I2C2->SR2;
		while((I2C2->SR1&0x40) == 0);				// RXNE
		regs[UT].byte.MSB = I2C2->DR;
		while((I2C2->SR1&0x40) == 0);				// RXNE
		I2C2->CR1 &= 0xFFFFFBFF;					// NACK
		I2C2->CR1 |= 0x200;
		regs[UT].byte.LSB = I2C2->DR;

		X1 = (regs[UT].ssi - regs[AC6].usi) * regs[AC5].usi / 32767;
		X2 = regs[MC].ssi * 2048 / (X1 + regs[MD].ssi);
		B5 = X1 + X2;
		T = (B5 + 8) / 16;

		print("\r\nT: %d", T);
		xSemaphoreGive(sem[SEM_TO_MEAS]);
	}
}

signed long P;
signed long B6;
signed long B3;
unsigned long B4;
signed long B7;
signed long X3;
void vPressMeasTask(void *pvParameters){


	for(;;){
		vTaskDelay(300);
		xSemaphoreTake(sem[SEM_TO_MEAS], portMAX_DELAY);

		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->DR = 0xEE;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		aaa = I2C2->SR2;
		while((I2C2->SR1&0x80) == 0);				// TXE
		I2C2->DR = 0xF4;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->DR = 0x34;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->CR1 |= 0x200;
		vTaskDelay(1);

		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->DR = 0xEE;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		aaa = I2C2->SR2;
		while((I2C2->SR1&0x80) == 0);				// TXE
		I2C2->DR = 0xF6;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->CR1 |= 0x400;							//ACK
		I2C2->DR = 0xEF;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		aaa = I2C2->SR2;
		while((I2C2->SR1&0x40) == 0);				// RXNE
		regs[UP].byte.MSB = I2C2->DR;
		while((I2C2->SR1&0x40) == 0);				// RXNE
		I2C2->CR1 &= 0xFFFFFBFF;					// NACK
		I2C2->CR1 |= 0x200;
		regs[UP].byte.LSB = I2C2->DR;

		B6 = B5 - 4000;
		X1 = (regs[B2].ssi * (B6 * B6 / 4096)) / 2048;
		X2 = regs[AC2].ssi * B6 / 2048;
		X3 = X1 + X2;
		B3 = (((regs[AC1].ssi*4 + X3)<<0) + 2)/4;
		X1 = regs[AC3].ssi * B6 / 8192;
		X2 = (regs[B1].ssi * ( B6 * B6 / 4096)) / 65536;
		X3 = ((X1 + X2) + 2) / 4;
		B4 = regs[AC4].usi * (unsigned long)(X3 + 32768) / 32768;
		B7 = ((unsigned long)regs[UP].usi - B3)*(50000>>0);

		if(B7 < 0x80000000){
			P = (B7 * 2)/B4;
		}
		else{
			P = (B7/B4) * 2;
		}

		X1 = (P/256) * (P/256);
		X1 = (X1*3038)/65536;
		X2 = (-7357 * P) / 65536;
		P = P + (X1 + X2 + 3791) / 16;

		print("\r\nP: %d", P*760/101325);
		xSemaphoreGive(sem[SEM_TO_MEAS]);
	}
}
