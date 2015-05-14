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
#include "base.h"
#include "task.h"
#include "sched.h"
#include "evt.h"

/* Initialize an Event */
void evt_init(evt_t *evt)
{
    list_init( & evt->waiting_list );
}

/*
    Make a task to wait for an Event releasing CPU.
    
    1. add the task to the waiting list of an event.
    2. remove the task from the run queue of the scheduler
*/
void evt_wait(evt_t *evt, task_t *task )
{
    list_node_t node; /* node resides in stack. */ 

    /* disable all interrupts including timer interrupt 
       to prevent this task from being context switched */
    base_paos_lock();

    node.data = sched_current_task() ;

    list_add_head( & evt->waiting_list, & node );

    sched_make_current_task_not_runnable();

    sched_task_yield();

    base_paos_unlock();
}

/* Wakeup an arbitrary task that is waiting on an Event */
void evt_wakeup(evt_t *evt)
{
    list_t      * waiting_list       = & evt->waiting_list;
    list_node_t * first_waiting; 
    task_t      * task;
    bool_t        has_waiting_task;

    base_paos_lock();

    first_waiting = waiting_list->head;

    has_waiting_task = (first_waiting != LIST_NODE_NULL(waiting_list));

    if ( has_waiting_task )
    {
        list_remove( waiting_list, first_waiting );

        /* Add the first waiting task to head of run queue */
        task = (task_t*) first_waiting->data;

        sched_make_task_runnable( task );

        sched_task_yield();
    }

    base_paos_unlock();
}

/* Wakeup all tasks that are waiting for an Event */
void evt_wakeup_all(evt_t *evt)
{
    list_t      * waiting_list       = & evt->waiting_list;
    list_node_t * first_waiting; 
    bool_t        has_waiting_task;

    base_paos_lock();

    first_waiting = waiting_list->head;

    has_waiting_task = (first_waiting != LIST_NODE_NULL(waiting_list));

    if ( has_waiting_task )
    {
        /* Add all waiting tasks to head of run queue */
        sched_make_tasks_runnable( waiting_list );

        /* initialize waiting_list */
        list_init( waiting_list );

        sched_task_yield();
    }

    base_paos_unlock();
}
