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
#include "pthread.h"

pthread_attr_t PThreadDefaultAttr = {
    CONF_DEFAULT_TASK_STACK_SIZE /* Default Stack Size */
};

int pthread_create( pthread_t  * thread,  pthread_attr_t * attr, void *(*start_routine)(void *),
                    void * arg)
{
    static u8 is_pthread_initialized = 0;

    if ( ! is_pthread_initialized ) {

        // initialized pthread system at the first call of pthread_create
        pthread_system_init();

        is_pthread_initialized = 1;
    }


    if ( attr == NULL ) {
        attr = & PThreadDefaultAttr;
    }

    return sched_task_create( (task_t*) thread,
                              start_routine,
                              arg,
                              attr->thread_stack_size );
}

pthread_t pthread_self( void ) {
   return *((pthread_t*)sched_current_task());
}


