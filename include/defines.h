/*
 * defines.h
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#define MAX_CMD_QUEUE_RECORD_LEN	32
#define MAX_CMD_QUEUE_SIZE			2

#define	MAX_PRINT_QUEUE_RECORD_LEN	80
#define	MAX_PRINT_QUEUE_SIZE		5

#define MAX_NUM_SEMAPHORES			5
#define SEM_USART1_RXNE				0
#define SEM_PRINT_QUEUE_SYNC		1

#define MAX_NUM_TASKS				10
#define	MAX_TASK_RECORD_LEN			40

#define MAX_NUM_VARS				128

#define	MAX_COMMAND_LEN				8
#define MAX_NUM_ARGS				2
#define MAX_ARG_LEN					16

#endif /* DEFINES_H_ */
