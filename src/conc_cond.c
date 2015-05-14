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
#include "sched.h"
#include "conc_cond.h"

/* Initialize a condition variable */
PA_RET conc_cond_init(conc_cond_t *cond)
{
    evt_init( & cond->signal_event );
    return 0;
}

/* Wake up a task that is waiting on a condition variable */
PA_RET conc_cond_signal(conc_cond_t *cond)
{
    evt_wakeup( & cond->signal_event );
    return -1;
}

/* Wake up all tasks that is wating on a condition variable */
PA_RET conc_cond_broadcast(conc_cond_t *cond)
{
    return -1;
}

/* Wait on a condition variable */
PA_RET conc_cond_wait(conc_cond_t *cond, conc_mutex_t *mutex)
{
    evt_wait( & cond->signal_event, sched_current_task() );

    return -1;
}

/* Try to wait on a condition variable for an amount of time */
PA_RET conc_cond_timedwait(conc_cond_t  *cond,  conc_mutex_t  *mutex,
                       const  struct timespec *abstime)
{
    return -1;
}

/* Destroy a condition variable */
PA_RET conc_cond_destroy(conc_cond_t *cond)
{
    evt_destroy( & cond->signal_event );
    return 0;
}

