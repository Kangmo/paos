/*  uart.h

    The UART Interface
*/

#ifndef _UART_H_
#define _UART_H_

#include "md_types.h"

/*
	Initialize UART
*/
void uart_init( u16 baud );

/*
	Send a byte
*/
void uart_tx_char( u8 ch);

/*
	Is transmit buffer empty?
*/
bool_t uart_tx_isempty( void );

/*
	Wait until all UART_tx requests are acutally sent
*/
void uart_tx_flush( void );

/*
	Receive a byte
*/
u8 uart_rx_char( void );

/*
	Is receive buffer empty?
*/
bool_t uart_rx_isempty( void );

/*
	Remove received data from rx buffer
*/
void uart_rx_flush( void );

/*
	finalize UART
*/
void uart_destroy( void );

#endif /* _UART_H_ */



