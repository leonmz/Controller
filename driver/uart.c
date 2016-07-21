/*
 * uart.c
 *
 *  Created on: Oct 30, 2014
 *      Author: Charlie
 */

//----------------include-----------------//
#include "system.h"
#include "sys/alt_irq.h"
#include <stdlib.h>
#include <stdio.h>
#include "..\inc\uart.h"
#include "..\inc\sopc.h"




//----------------Func Declear-----------------//
static int uart_send_byte(unsigned char data);
static void uart_send_string(unsigned int len, unsigned char *str);
static int uart_init(void);
static void uart_ISR(void *context);
static int set_baudrate(unsigned int baudrate);
//----------------Struct Init-----------------//
UART_T uart = {
		.receive_flag = 0,
		.receive_count = 0,
		.send_byte = uart_send_byte,
		.send_string = uart_send_string,
		.init = uart_init,
		.baudrate = set_baudrate
		};

int uart_send_byte(unsigned char data)
{
	UART->TXDATA.BITS.TRANSMIT_DATA = data;
	while (!UART->STATUS.BITS.TRDY);
	return 0;
}

void uart_send_string(unsigned int len, unsigned char *str)
{
	while (len--)
	{
		uart_send_byte(*str++);
	}
}

int uart_init(void)
{
	set_baudrate(9600);
	UART->CONTROL.BITS.IRRDY = 1;
	UART->STATUS.WORD = 0;
	alt_ic_isr_register(UART_IRQ_INTERRUPT_CONTROLLER_ID, UART_IRQ, uart_ISR,
		NULL, 0x0);
	return 0;
}

static void uart_ISR(void *context)
{
	while (!(UART->STATUS.BITS.RRDY));
	uart.receive_buffer[uart.receive_count++] = UART->RXDATA.BITS.RECEIVE_DATA;
	/*
	if (uart.receive_buffer[uart.receive_count - 1] == '\n')
	{
		uart.receive_buffer[uart.receive_count] = '\0';
		uart.receive_count = 0;
		uart.receive_flag = 1;
	}*/

	if(uart.receive_count==8)
	{
		uart.receive_count = 0;
		uart.receive_flag = 1;
	}
}

int set_baudrate(unsigned int baudrate)
{
	UART->DIVISOR.WORD = (unsigned int) (ALT_CPU_FREQ / baudrate + 0.5);
	return 0;
}

