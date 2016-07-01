/* ... */

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "usart.h"



void USART1_IRQHandler(void) {
	char c;
	
	static int count = 0;
	static char rcv[33];
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		c = (char)USART_ReceiveData(USART1);
		
		rcv[count] = c;
		count++;
		
		if((count == 32) || (c == '\r')) {
			rcv[count] = '\0';
			
 			if(!(strcmp(rcv, "GBL\r"))) {		// go to bootloader
				/* write constant to ram */
 				*((unsigned long *)0x2000FFF0) = 0xDEADBEEF;
				
				/* do a system reset */
 				SCB->AIRCR = ((uint32_t)0x05FA0000) | (u32)0x04;				
			}
			
			count = 0;
			memset(rcv, 0, 33);
		}

		//USART_SendData(USART1, c);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}


/* here we redirect stdio to USART1, printf */
/* TODO TASK GETCHAR_PROTOTYPE */
// #define PUTCHAR_PROTOTYPE	int __io_putchar(int ch)

// // PUTCHAR_PROTOTYPE
// {
// 	USART_SendData(USART1, (uint8_t) ch);
// 
// 	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) 
// 	{}
// 
// 	return ch;
// }

void usart1_init(void) {
	USART_InitTypeDef       usart_i;
	GPIO_InitTypeDef        gpio_i;

	GPIO_PinRemapConfig(GPIO_Remap_USART1,DISABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,   ENABLE);

	/* TX */
	gpio_i.GPIO_Pin   = GPIO_Pin_9;
	gpio_i.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_i.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio_i);

	/* RX */
	gpio_i.GPIO_Pin   = GPIO_Pin_10;
	gpio_i.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_i.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio_i);

	/* 115200 8n1 */
	usart_i.USART_BaudRate              = 115200;
    	usart_i.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;
    	usart_i.USART_Mode       	    = USART_Mode_Rx | USART_Mode_Tx;
    	usart_i.USART_Parity		    = USART_Parity_No;
    	usart_i.USART_StopBits		    = USART_StopBits_1;
    	usart_i.USART_WordLength            = USART_WordLength_8b;

	USART_Init(USART1, &usart_i);

	/* enable RXNE interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);

	USART_Cmd(USART1,ENABLE);
}

void usart1_putc(char c){
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, c);
	return;
}

void usart1_puts(char *str){
	while(*str != 0) {
		usart1_putc(*(str++));
	}
	return;
}

uint8_t usart1_getc(void) {
	while((USART1->SR & USART_SR_RXNE) == 0);

	return USART1->DR & 0x1FF;
}


