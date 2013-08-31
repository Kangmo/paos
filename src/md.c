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

#include <avr/io.h>
#include <avr/interrupt.h>
#include "md.h"


/* Enable Interrupts */
void md_int_enable( void )
{
    sei();
}

/* Disable Interrupts */
void md_int_disable( void )
{
    cli();
}


/* Delay for millisecond(s)
   Note : _delay_ms  function of avr-gcc does not support delaying long period of time */
void md_delay_ms(double __ms)
{
    uint16_t __ticks;
    double __tmp = ((F_CPU) / 4e3) * __ms;
    if (__tmp < 1.0) {
        __ticks = 1;
        _delay_loop_2(__ticks);
    } else {
        while (__tmp > 0) {
        if (__tmp <= 65535) {
                __ticks = (uint16_t)__tmp;
                _delay_loop_2(__ticks);
                __tmp = 0;
            }
            else
            {
                _delay_loop_2(65535);
                __tmp -= 65535;
            }       
        }
    }
}

/* Turn on the 8 bit timer interrupt of ATMega128 */
void timer_8_on( void )
{

/*
  TCCR0 --> NORMAL TIMER FREQUENCE SELECTOR
  0 -> TIMER/COUNTER STOP

  TCCR0 (PRESCAL)    16MHz
  ---------------------------------
  1 (1/   1)     ->   62nsec
  2 (1/   8)     ->  500nsec
  3 (1/  32)     ->   2usec
  4 (1/  64)     ->   4usec
  5 (1/ 128)     ->   8usec
  6 (1/ 256)     ->  16usec
  7 (1/1024)     ->  64usec
*/
    TCNT0 = 0x0;
    TCCR0 = 0x07;
    TIMSK |= 1 << TOIE0;
}

/*
void timer_16_on( void )
{

  TCCR1B = 3; // prescale 0~7

  TIMSK |= 1 << TOIE1;

  TCNT1H = 0x0; // xx msec up counting
  TCNT1L = 0x0;
}
*/

/* Start Timer Interrupt for Task Switching */
void md_start_timer( void )
{
    timer_8_on();

    sei();                    /* Global Interrupt Enable */
}

/*
   Initialize the Task stack

   stack_low    - Low address of the task stack
   stack_high   - High address of the task stack
   func         - The function to run as Task
   return       - new top of stack after initialized

structure of task stack
=============================================================================================
(bottom) high addr          PCL
                            PCH
                            r31                        ; r31 is used to pop SREG
                            r30                        ; r30 is used to check stack signature
                            SREG ( Status Register )
                            0xFE                       ; Stack Signature 1
                            0xCA                       ; Stack Signature 2
                            r29
                            r28
                            ...
                            r1
                            r0
(top) low addr              RAMPZ
*/

md_stackp_t md_init_task_stack(md_stackp_t stack_low, md_stackp_t stack_high, void* func)
{
    int i = 0;

    u8   * stack  = (u8*) stack_high;
    u16    ret_PC = (u16) func;       /* return PC(program counter) */

    /* Initialize the Return address */

    *stack -- = (u8) (ret_PC) ;       /* PCL ( Low Byte of PC )  */
    *stack -- = (u8) (ret_PC >> 8);   /* PCH ( High Byte of PC ) */

    *stack -- = (u8) 0;               /* r31   := 0 */
    *stack -- = (u8) 0;               /* r30   := 0 */

    *stack -- = (u8) 0x80;            /* SREG := 0x80  ( Global Interrupt Enable ) */

    *stack -- = (u8) 0xCA;            /* Stack Signature - CA */
    *stack -- = (u8) 0xFE;            /* Stack Signature - FE */

    /* initialize r0 ~ r29 register on stack */

    for (i=0; i<=29; i++)
    {
        *stack-- = (u8) 0;            /* Rxx := 0 */
    }

    *stack-- = 0;                     /* RAMPZ := 0 */

    PA_ASSERT( (md_stackp_t)stack > stack_low );

    return (md_stackp_t) stack;
}



