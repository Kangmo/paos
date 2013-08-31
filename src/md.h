/*  md.h
    
    MD( Machine Dependent ) Layer

    This layer has every code fraction that depends on specific hardware.

    In order to port PAOS to a new platform, all you need to do is 
    just to implement md.c and md_asm.S

    +-------------------------------------+
    |                                     |
    |       ... Other Layers ...          |
    |                                     |
    +-------------------------------------+
    | BASE ( Machine Independent Layer )  |
    +-------------------------------------+
    | MD ( Machine Dependent Layer )      | <=== Here
    +-------------------------------------+
*/
#ifndef __MD_H__
#define __MD_H__ (1)

#include "config.h"
#include <avr/interrupt.h>
#include <util/delay.h>

/* Delay for millisecond(s)
   Note : _delay_ms  function of avr-gcc does not support delaying long period of time */
void md_delay_ms(double b);

/* Delay for microseconds(s) */
#define md_delay_us(us) _delay_us((us))

/* Enable Interrupts */
void md_int_enable( void );

/* Disable Interrupts */
void md_int_disable( void );

/* The type of stack pointer of CPU */
typedef void* md_stackp_t;

/* Start Timer Interrupt for Task Switching */
void md_start_timer( void );

/*
   Initialize the Task stack
*/
md_stackp_t md_init_task_stack(md_stackp_t stack_low, md_stackp_t stack_high, void* func);


/* Called by currently running task to execute context switching voluntarily
   Body is defined in md_asm.S
*/
void md_switch_context_by_task( void );

#endif /* __MD_H__ */



