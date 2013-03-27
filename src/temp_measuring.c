/*
 * temp_measuring.c
 *
 *  Created on: Mar 27, 2013
 *      Author: nemo
 */

#include "header.h"

void vTempMeasTask(void *pvParameters){
	signed long X1;
	signed long X2;
	signed long B5;

	for(;;){
		vTaskDelay(mks2tick(TEMPERATURE_MEAS_PERIOD));
		xSemaphoreTake(sem[SEM_TO_MEAS], portMAX_DELAY);

		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->DR = 0xEE;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		I2C2->SR2;									// fake read register
		while((I2C2->SR1&0x80) == 0);				// TXE
		I2C2->DR = 0xF4;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->DR = 0x2E;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->CR1 |= 0x200;

		vTaskDelay(mks2tick(4500));

		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->DR = 0xEE;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		I2C2->SR2;									// fake read register
		while((I2C2->SR1&0x80) == 0);				// TXE
		I2C2->DR = 0xF6;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->CR1 |= 0x400;							//ACK
		I2C2->DR = 0xEF;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		I2C2->SR2;									// fake read register
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

		//print("\r\nT: %d", T);
		xSemaphoreGive(sem[SEM_TO_MEAS]);
	}
}
