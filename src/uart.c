/*
    <SOURCE-LICENSE>
        [ PA(Peter-Anna) OS ] ========================================

          Peter-Anna OS is a real time OS.
          Peter-Anna OS is made by Peter and Anna.
          Peter and Anna lives in South Korea.
          They love each other so much.

          Peter usually develops PAOS,
          Anna usually feeds hungry Peter.
          Peter sometimes makes money,
          Anna sometimes makes cookie.

          Like shared memory region,
          PAOS is where they live in together.
          They belong to PAOS.
          PAOS belongs to them.

                               03/29/2006
                               Peter and Anna ( kangmo.kim@gmail.com )
        [ License Information ] ======================================
         
          Dual License
            1. GPL
            2. PAOS Commercial License
             
          License Selection Algorithm             
            IF You can open your whole source code
              You are under GPL license
            ELSE
              You are under PAOS Commercial License
            FI
                                
        [ Information on Layering Architecture of PAOS ] =============

          Files in lower layer are not allowed to 
          use,include files in upper layer.

          user    ; user program layer      ; main.c
          pthread ; pthread layer           ; pthread.c
          conc    ; concurrency layer       ; conc_mutex.c conc_cond.c
          evt     ; event layer             ; evt.c
          sched   ; scheduler layer         ; sched.c
          task    ; task layer              ; task.c
          base    ; base layer              ; base.c list.c debug.c
          md      ; machine dependent layer ; md.c md_asm.S uart.c
          config  ; configuration layer     ; config.h
                                
    </SOURCE-LICENSE>
    $Id$
*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "debug.h"
#include "config.h"
#include "md.h"
#include "base.h"

#include "uart_common.h"

// initialize serial communication
void uart_init( u16 baud )
{
    u8 dummy;
    u16 ubrr;
    
    ubrr = BAUD2UBRR(baud);

        // Set UBRR register
    UBRRH = (u8) ( ubrr >> 8 );
    UBRRL = (u8) ( ubrr );

    // Enable reciever and transmitter
    UCSRB = (1 << RXEN) | ( 1<<TXEN);

    // Set frame format : 8data, 1stop bit
    UCSRC = (0<<USBS) | (3<<UCSZ0);

    ACSR=0x80;

    // dummy read
    dummy=UDR;      
}

// has a char in rx buffer ?
bool_t uart_tx_isempty( void )
{
    return (UCSRA & 0x20 ) == 0x00;
}


// send a char
void uart_tx_char(u8 data )
{
    base_paos_lock();

    // Wait for empty transmit buffer;
    while( uart_tx_isempty() );

    // Put data into buffer, sends the data.
    UDR = data;

    base_paos_unlock();
}

// has a char in rx buffer ?
bool_t uart_rx_isempty( void )
{
    return ( UCSRA & (1<<(RXC)) ) == 0x00;
}

// receive a char ( return 0 if no char is in rx buffer )
u8 uart_rx_char( void )
{
    u8 ch;

    base_paos_lock();

    if ( uart_rx_isempty() )
    {
        ch=0;
    }
    else
    {
        ch=UDR;
    }

    base_paos_unlock();

    return ch;
}

/*
    list of unsupported functions
    --------------------------------------------------------------------
    uart_tx_flush
    uart_rx_flush
*/

