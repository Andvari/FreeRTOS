/*
 * lib.h
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#ifndef LIB_H_
#define LIB_H_

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "globalvars.h"

void print(char *);
int strlen(char *);
int strcmp(char *, char *);
int itoa(int, char *);
int ftoa(float, char *);

#endif /* LIB_H_ */
