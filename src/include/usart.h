#ifndef __USART_H__
#define __USART_H__

#include <stdio.h>
#include "stm32f10x.h"

extern void usart1_init(void);
extern void usart1_putc(char c);
extern void usart1_puts(char *str);
extern uint8_t usart1_getc(void);

#endif // __USART_H__
