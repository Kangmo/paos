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
         
          Apache v2 License
                                
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
#include "config.h"
#include "md.h"
#include "base.h"
#include "task.h"
#include <avr/interrupt.h>

/* The currently running task */
extern task_t * CurrTask;

/*  Run the main function of the current Task */
void task_func_wrapper( void )
{
    task_ret_t exit_code ;

    PA_ASSERT( CurrTask != (task_t*) 0 );

    exit_code = (*CurrTask->task_func)( CurrTask->task_arg );

    CurrTask->exit_code = exit_code;
}

/* Initialize a Task */
void task_init( task_t * task, task_func_t task_func, task_arg_t task_arg, md_stackp_t stack_low, md_stackp_t stack_high)
{
    /* Initialize the context of a new Task into the stack */
    task->task_SP   = md_init_task_stack(stack_low, stack_high, task_func_wrapper );
    task->task_stack_low  = stack_low;
    task->task_stack_high = stack_high;
    task->task_func = task_func;
    task->task_arg  = task_arg;
    task->exit_code = (task_ret_t)0;                    
    task->task_tick = CONF_DEFAULT_TASK_TICK; /* after 10 times of timer ISR, this task is switched */
    task->task_node.data = task ;
    base_paos_init_lock_context( & task->lock_context );

    /* no need to initialize task->task_node.prev , next */
}

/* Initialize the Main Task */
void task_init_main( task_t * task, md_stackp_t stack_low, md_stackp_t stack_high )
{

    task->task_SP   = NULL; /* SP is set on the first context switch */
    task->task_stack_low  = stack_low;
    task->task_stack_high = stack_high;
    task->task_func = NULL; /* never referenced (main task is already running) */
    task->task_arg  = NULL; /* never referenced (main task is already running) */
    task->exit_code = (task_ret_t)0;                    
    task->task_tick = CONF_DEFAULT_TASK_TICK;
    task->task_node.data = task ;

    /* no need to initialize task->task_node.prev , next
       no need to initialize task->lock_context  ( set on the first context switch ) */
}

/*  Check stack overrun or underrun

    Return
      - 0 : task stack is not ok
      - Otherwise : task stack is ok
 */
bool_t task_stack_ok( task_t * task ) {
    return ( task->task_stack_low <= task->task_SP ) &&
           ( task->task_SP <= task->task_stack_high );
}

