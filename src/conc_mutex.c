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
#include "conc_mutex.h"

/* Initialize a Mutex */
PA_RET conc_mutex_init(conc_mutex_t *mutex)
{
    mutex->locked_task = NULL;
    evt_init( & mutex->mutex_unlock_event );
    return 0;
}

/* Acquire a lock on a Mutex */
PA_RET conc_mutex_lock(conc_mutex_t *mutex)
{
    task_t * current_task;

    base_paos_lock(); /* To test and set mutex->locked_task
                         without context switching nor interrupt handling */

    if ( mutex->locked_task == NULL ) { /* The mutex is not locked yet */
        mutex->locked_task = sched_current_task();
    } else {                            /* The mutex is already locked */
        current_task = sched_current_task();

        /* The current task yields while md_paos_lock is called */
        evt_wait( &mutex->mutex_unlock_event, current_task );
        PA_ASSERT( mutex->locked_task == NULL );
        mutex->locked_task = current_task;
    }

    base_paos_unlock();
    return 0;
}


#if defined ( CONFIG_MUTEX_TIMEDLOCK )
PA_RET conc_mutex_timedlock(conc_mutex_t * mutex, const struct timespec * abs_timeout) {
    PA_ASSERT(0); // Need to Implement 
    return -1;
}
#endif /* CONFIG_MUTEX_TIMEDLOCK */


#if defined ( CONFIG_MUTEX_TRYLOCK )
/* Try to acquire a lock on a Mutex */
PA_RET conc_mutex_trylock(conc_mutex_t *mutex)
{
    PA_RET ret = -1;

    base_paos_lock(); /* To test and set mutex->locked_task
                         without context switching nor interrupt handling */

    if ( mutex->locked_task == NULL ) { /* The mutex is not locked yet */
        mutex->locked_task = sched_current_task();
        ret = 0;
    } else {                            /* The mutex is already locked */
        /* Failed to acquire a lock on the mutex 
           nothing to do : ret is already set to -1 */
    }

    base_paos_unlock();

    return ret;
}
#endif /* CONFIG_MUTEX_TRYLOCK */

/* Release a lock on a Mutex */
PA_RET conc_mutex_unlock(conc_mutex_t *mutex)
{
    PA_RET ret;

    base_paos_lock();  /* To test and set mutex->locked_task
                         without context switching nor interrupt handling */

    /* Has the current task acquired a lock on the mutex ? */ 
    if ( mutex->locked_task == sched_current_task() ) { 
        mutex->locked_task = NULL;
        ret = 0;
    }
    else /* Current task did not lock the mutex but tried to unlock it. */ 
    {
        ret = -1;
    }

    /* Wake up a task that is waiting for the mutex to be unlocked */
    evt_wakeup(&mutex->mutex_unlock_event);

    base_paos_unlock();

    return ret;
}

/* Destroy a Mutex */
PA_RET conc_mutex_destroy(conc_mutex_t *mutex)
{
    /* nothing to do but make sure that there is no locked tasks */
    return mutex->locked_task != NULL;
}
