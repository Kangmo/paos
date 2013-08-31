/*
    uart-common.h
    common header file for uart modules
*/

// Convert Baud rate to UBRR register value
#define BAUD2UBRR( baud ) (F_CPU/16/(baud) - 1)

/////////////////////////////////////////////////////
// CPU specific register definition
/////////////////////////////////////////////////////
#if defined( __AVR_ATmega128__ )

#define UBRRH           (UBRR0H)
#define UBRRL           (UBRR0L)
#define UDR             (UDR0)
#define UCSRA           (UCSR0A)
#define UCSRB           (UCSR0B)
#define UCSRC           (UCSR0C)
#define USART_TX_vect   (USART0_TX_vect)
#define USART_RX_vect   (USART0_RX_vect)
#define USART_UDRE_vect (USART0_UDRE_vect)

#else

#error "Unsupported CPU by uart-interrupt.c"

#endif /* __AVR_ATmega128__ */
