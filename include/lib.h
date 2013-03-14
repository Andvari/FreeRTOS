/*
 * lib.h
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#ifndef LIB_H_
#define LIB_H_

#include "header.h"

void print(char *, ...);

char *	itoa(int, char *, int);
char *	ftoa(float, char *);
int		atoi(char *);
char *	get_token(char *, char *, char);
void	parse_string(char *, char *, char *, char *);
void	dump_line(char *, char *);
void	dump(char *, int);
char	hex_symbol(char);

#endif /* LIB_H_ */
