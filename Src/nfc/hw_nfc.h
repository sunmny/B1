#ifndef UART_APP_H
#define UART_APP_H

#include "stm32l4xx.h"
//#define ERROR	1
#define	OK		0
#define True	1
#define False	0
extern void Uart_App(void);
void Delay_100us(unsigned int delay_time);		//0.1ms*delay_time

#endif