/*
 * press_measuring.c
 *
 *  Created on: Mar 27, 2013
 *      Author: nemo
 */

#include "header.h"

void vPressMeasTask(void *pvParameters){
	signed long X1;
	signed long X2;
	signed long B6;
	signed long B3;
	unsigned long B4;
	signed long B5;
	signed long B7;
	signed long X3;

	int delay_table_mks[] = {4500, 7500, 13500, 25500};

	for(;;){
		vTaskDelay(mks2tick(PRESSURE_MEAS_PERIOD));
		xSemaphoreTake(sem[SEM_TO_MEAS], portMAX_DELAY);

		I2C2->CR1 |= 0x100;
		while((I2C2->SR1&0x1) == 0);				// SB
		I2C2->DR = 0xEE;
		while((I2C2->SR1&0x2) == 0);				// ADDR
		I2C2->SR2;									// fake read register
		while((I2C2->SR1&0x80) == 0);				// TXE
		I2C2->DR = 0xF4;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->DR = 0x34;
		while((I2C2->SR1&0x4) == 0);				// BTF
		I2C2->CR1 |= 0x200;
		vTaskDelay(mks2tick(delay_table_mks[PRESSURE_MODE]));

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
		regs[UP].byte.MSB = I2C2->DR;
		while((I2C2->SR1&0x40) == 0);				// RXNE
		I2C2->CR1 &= 0xFFFFFBFF;					// NACK
		I2C2->CR1 |= 0x200;
		regs[UP].byte.LSB = I2C2->DR;

		X1 = (regs[UT].ssi - regs[AC6].usi) * regs[AC5].usi / 32767;
		X2 = regs[MC].ssi * 2048 / (X1 + regs[MD].ssi);
		B5 = X1 + X2;
		B6 = B5 - 4000;
		X1 = (regs[B2].ssi * (B6 * B6 / 4096)) / 2048;
		X2 = regs[AC2].ssi * B6 / 2048;
		X3 = X1 + X2;
		B3 = (((regs[AC1].ssi*4 + X3)<<PRESSURE_MODE) + 2)/4;
		X1 = regs[AC3].ssi * B6 / 8192;
		X2 = (regs[B1].ssi * ( B6 * B6 / 4096)) / 65536;
		X3 = ((X1 + X2) + 2) / 4;
		B4 = regs[AC4].usi * (unsigned long)(X3 + 32768) / 32768;
		B7 = ((unsigned long)regs[UP].usi - B3)*(50000>>PRESSURE_MODE);

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

