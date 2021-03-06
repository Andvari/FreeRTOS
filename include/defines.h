/*
 * defines.h
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#define SRAM_START_ADDRESS			0x20000000
#define	SRAM_SIZE					0x8000


#define MAX_CMD_QUEUE_RECORD_LEN	32
#define MAX_CMD_QUEUE_SIZE			2

#define	MAX_PRINT_QUEUE_RECORD_LEN	80
#define	MAX_PRINT_QUEUE_SIZE		5

#define MAX_NUM_SEMAPHORES			5
#define SEM_USART1_RXNE				0
#define SEM_PRINT_QUEUE_SYNC		1
#define SEM_TO_MEAS					2
#define	SEM_I2C2_SB					3

#define MAX_NUM_TASKS				10
#define	MAX_TASK_RECORD_LEN			40

#define MAX_NUM_VARS				128

#define	MAX_COMMAND_LEN				8
#define MAX_NUM_ARGS				2
#define MAX_ARG_LEN					16

/*
 * BMP085 defines
 */
#define	BMP085_I2C	I2C2
#define	BMP085_CONFIG_REGS_ADDR		(0xAA)
#define	BASE_NUM_REGS	11
#define	NUM_REGS		13
#define	AC1				0
#define	AC2				1
#define	AC3				2
#define	AC4				3
#define	AC5				4
#define	AC6				5
#define	B1				6
#define	B2				7
#define	MB				8
#define	MC				9
#define	MD				10
#define	UT				11
#define	UP				12

#define PRESSURE_MODE	0

#define	PRESSURE_MEAS_PERIOD	10000		/*mks*/
#define	TEMPERATURE_MEAS_PERIOD	1000000		/*mks*/

#endif /* DEFINES_H_ */
