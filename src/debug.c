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
#include "debug.h"
#include "base.h"

#include <stdarg.h>
#include <stdio.h>

#include "uart.h"

/* Initialize the USART trace driver */
void td_serial_init(void) {
    /* uart_init(9600); */
    uart_init(57600);
}

/* The trace driver of USART */
trace_driver_t td_serial = {
    & td_serial_init,
    & uart_tx_char
};

/*
trace_driver_t td_lcd = {
    & lcd_init,
    & lcd_putchar
};
*/

/* The trace driver that is responsible for printing trace messages */
trace_driver_t * tdriver = NULL;

/* Initialize the tracing system with a trace driver */
void trace_init(trace_driver_t * driver)
{
    tdriver = driver;
    (*tdriver->init)();
}

/* Print a trace message onto the current trace driver */
void trace_puts(u8 * message )
{
    if ( tdriver != NULL )
    {
        while ( *message )
           (*tdriver->putchar)( *message++ );
    }
}

/*  The filter of trace mssages

    Determines whether to filter a message or not

    return 0 if not to filter message
           otherwise filter message
*/
u8 trace_filter(trace_module_t module, trace_level_t level)
{
    if ( module == 0 && level == 0 )
    {
        return 0;
    }

    return 0;
}

/*  Print a trace message ( printf like version )
*/
void tracef(trace_module_t module, trace_level_t level, char* format, ... )
{
#define MAX_BUF 128

    char buf[MAX_BUF];
    va_list list;

    if ( ! trace_filter(module,level ) )
    {
        va_start(list, format);
        vsnprintf(buf, MAX_BUF, format, list );

        trace_puts( buf );

        va_end(list);
    }
}


/*  Print an assertion expression, a source file, a line number 
    where the assertion failure has occurred
 */
void trace_assertion( s8 * exp, s8 * file, u16 line )
{
    base_paos_lock();
    tracef( 0, 0, "[%s:%d] ASSERTION FAILURE : %s", file, line, exp );
    base_paos_unlock();
}



