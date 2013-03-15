/*
 * globalvars.h
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "FreeRTOS.h"
#include "defines.h"
#include "semphr.h"
#include "queue.h"

#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

char cmd_queue[MAX_CMD_QUEUE_SIZE][MAX_CMD_QUEUE_RECORD_LEN];
int idx_rd_cmdqueue;
int idx_wr_cmd_queue;
int idx_cmd_queue;

char print_queue[MAX_PRINT_QUEUE_SIZE][MAX_PRINT_QUEUE_RECORD_LEN];
int idx_rd_print_queue;
int idx_wr_print_queue;

int idx_rd_cmd_queue;
int idx_wr_cmd_queue;
int len_wr_cmd_queue;

char vars[MAX_NUM_VARS];

xSemaphoreHandle	sem[MAX_NUM_SEMAPHORES];
xQueueHandle		queue_print_sync;

portCHAR task_list[MAX_NUM_TASKS*MAX_TASK_RECORD_LEN];

int timer6_counter;

#endif /* GLOBALVARS_H_ */
