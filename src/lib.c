/*
 * lib.c
 *
 *  Created on: Mar 7, 2013
 *      Author: nemo
 */

#include "./include/lib.h"

#define	MAX_LINE_SIZE	80

void print(char *format, ...){
	int i;
	int len;
	int state;
	char line[MAX_LINE_SIZE];
	char item[16];
	va_list args;

	i		=	0;
	len		=	0;
	state	=	0;

	va_start(args, *format);
	do{
		if(format[i] == '%'){
			if(state == 0){
				state = 1;
			}
			else{
				state = 0;
				line[len++] = format[i];
			}
		}
		else{
			if(state == 0){
				line[len++] = format[i];
			}
			else{
				state = 0;
				switch(format[i]){
								case	'd':	strcpy(&line[len], itoa(va_arg(args, int), item, 10));
												break;
								case	'f':	strcpy(&line[len], ftoa(va_arg(args, double), item));
												break;
								case	'x':	strcpy(&line[len], itoa(va_arg(args, int), item, 16));
												break;
								case	'c':	line[len++] = va_arg(args, int);
												line[len] = 0;
												break;
								case	's':	strcpy(&line[len], va_arg(args, char *));
												break;
								default:
												break;
				}
				len = strlen(line);
			}
		}
	}while(format[i++]!=0);

	va_end(args);

	xSemaphoreTake(sem[SEM_PRINT_QUEUE_SYNC], portMAX_DELAY);
	if((idx_wr_print_queue+1)%MAX_PRINT_QUEUE_SIZE != idx_rd_print_queue){
		i=0;
		do{
			print_queue[idx_wr_print_queue][i] = line[i];
		}while(line[i++]!=0);

		do{
		}while(xQueueSend(queue_print_sync, NULL, portMAX_DELAY) != pdTRUE);

		idx_wr_print_queue = (idx_wr_print_queue+1)%MAX_PRINT_QUEUE_SIZE;
	}

	xSemaphoreGive(sem[SEM_PRINT_QUEUE_SYNC]);

	vTaskDelay(1);
}

char *itoa(int val, char *dst, int radix){
	int i=0, j;
	unsigned int tmp;
	char a;

	if(val == 0){
		switch (radix){
		case	16:	dst[0] = '0';
					dst[1] = 'x';
					dst[2] = '0';
					dst[3] = '0';
					dst[4] = '0';
					dst[5] = '0';
					dst[6] = '0';
					dst[7] = '0';
					dst[8] = '0';
					dst[9] = '0';
					dst[10]= 0;
					break;
		default:	dst[0] = '0';
					dst[1] = 0;
					break;
		}
	}
	else{

		tmp = val;
		if((val<0)&&(radix==10)) tmp = -val;

		while(tmp > 0){
			a = tmp%radix + 48;
			if(a>57)a+=7;
			dst[i++] = a;
			tmp /= radix;
		}

		if(radix == 16){
			while(i<8){
				dst[i++] = '0';
			}
			dst[i++] = 'x';
			dst[i++] = '0';
		}
		else{
			if( val < 0 )dst[i++] = '-';
		}

		dst[i] = 0;

		for(j=0; j<i/2; j++){
			a = dst[j];
			dst[j] = dst[i-j-1];
			dst[i-j-1] = a;
		}
	}

	return (dst);
}

char *ftoa(float val, char *dst){
	int b;
	int sz;

	b = (val - (int)val)*100000;
	if(b<0) b = -b;

	itoa((int)val, dst, 10);
	sz = strlen(dst);
	dst[sz++] = '.';

	if(b<10000)	dst[sz++] = '0';
	if(b<1000)	dst[sz++] = '0';
	if(b<100)	dst[sz++] = '0';
	if(b<10)	dst[sz++] = '0';

	return (dst);
}

int atoi(char *str){
	int		i;
	int		val;
	char	ofst;
	char	radix;

	ofst	=	0;
	radix	=	10;

	if((str[0]=='0')&&((str[1]=='x')||(str[1]=='X'))){
		ofst	=	2;
		radix	=	16;
	}

	i=0;
	val = 0;
	if(radix == 10){
		while((str[ofst+i] >= 48) && (str[ofst+i] <= 57)){
			val = val*10 + str[ofst+i++] - 48;
		}
	}
	else{
		while(((str[ofst+i] >= 48) && (str[ofst+i] <= 57)) || ((str[ofst+i] >= 65) && (str[ofst+i] <= 70))){
			if((str[ofst+i] >= 48) && (str[ofst+i] <= 57)){
				val = val*16 + str[ofst+i++] - 48;
			}
			else{
				val = val*16 + str[ofst+i++] - 65 + 10;
			}
		}
	}

	return (val);
}

void parse_string(char *str, char *command, char *arg1, char *arg2){
	command[0] = 0;
	arg1[0] = 0;
	arg2[0] = 0;

	if(str != NULL)str = get_token(str, command,	' ');
	if(str != NULL)str = get_token(str, arg1,		' ');
	if(str != NULL)str = get_token(str, arg2,		' ');
}

char *get_token(char *str, char *dst, char delimiter){
	int i;

	i=0;
	do{
		dst[i] = str[i];
		if(str[i] == delimiter){
			dst[i]=0;
			if(str[i+1] == 0)	return (NULL);
			else				return (&str[i+1]);
		}
	}while(str[i++]!=0);

	return (NULL);
}

void dump_line(char *bytes, char *line){
	int i, j, k, l;
	char nibble;

	k=0;

	for(i=0; i<8; i++){
		nibble = ((int)bytes>>(7-i)*4)&0xF;
		line[k++] = hex_symbol(nibble);
	}

	line[k++] = ':';
	line[k++] = ' ';

	i=0;
	for(j=0; j<2; j++){
		for(l=0; l<8; l++){
			line[k++] = hex_symbol(bytes[i  ]>>4);
			line[k++] = hex_symbol(bytes[i++]   );
			line[k++] = ' ';
		}
		line[k++] = ' ';
	}

	for(i=0; i<16; i++){
		if((bytes[i] < 32)||(bytes[i] > 126))	line[k++] = '.';
		else					line[k++] = bytes[i];
	}

	line[k++] = 0;

}

void dump(char *bytes, int size){
	char line[80];
	union{
		char			*pchar;
		unsigned int	integer;
	}addr;

	addr.pchar		=	bytes;
	addr.integer	&=	0xFFFFFFF0;

	while(size > 0){
		dump_line(addr.pchar, line);
		print("\r\n%s", line);

		size -= 16;
		addr.integer	+=	0x10;
	}
}

char hex_symbol(char nibble){
	nibble &= 0x0F;
	if(nibble<=9)	return (nibble+48);
	else			return (nibble-10+65);
}
