/* conc_mutex.h
   
   Implementation of mutex of Pthread interface

*/
#ifndef __CONC_MUTEX_H__
#define __CONC_MUTEX_H__ (1)

#include "md_types.h"
#include "task.h"
#include "evt.h"

/* Mutex

   A data structure for implementing the pthread mutex.

   This structure is not shown to the pthread layer.
   By hiding implementation specific codes to user API, 
   future implementation can be easily changed 
   without notifying it to API users.
*/
typedef struct conc_mutex_t {
    /* The task that has acquired a lock on this mutex */
	task_t * locked_task;

    /* The event of mutex unlock 
       Tasks which failed to acquire a lock on this mutex,
       start to wait on this event releasing CPU to the scheduler.
    */         
	evt_t    mutex_unlock_event;
} conc_mutex_t ;

/* Initialize a Mutex */
PA_RET conc_mutex_init(conc_mutex_t *mutex);

/* Acquire a lock on a Mutex */
PA_RET conc_mutex_lock(conc_mutex_t *mutex);

/* Try to acquire a lock on a Mutex */
PA_RET conc_mutex_trylock(conc_mutex_t *mutex);

/* Release a lock on a Mutex */
PA_RET conc_mutex_unlock(conc_mutex_t *mutex);

/* Destroy a Mutex */
PA_RET conc_mutex_destroy(conc_mutex_t *mutex);

#endif /* __CONC_MUTEX_H__ */

