/* conc_cond.h

   Implementation of condition variable of Pthread interface

*/

#ifndef __CONC_COND_H__
#define __CONC_COND_H__ (1)

#include "evt.h"
#include "conc_mutex.h"

/* The Condition Variable
 
   A data structure for implementing the pthread condition variable.

   This structure is not shown to the pthread layer.
   By hiding implementation specific codes to user API, 
   future implementation can be easily changed 
   without notifying it to API users.
*/
typedef struct conc_cond_t {
	evt_t signal_event;       /* the signal event of the condition */
} conc_cond_t;

/* Time Duration */
struct timespec {
   u16 tv_sec;     /* second(s)       */
   u32 tv_usec;    /* micro second(s) */
};

/* Initialize a condition variable */
PA_RET conc_cond_init(conc_cond_t *cond);

/* Wake up a task that is waiting on a condition variable */
PA_RET conc_cond_signal(conc_cond_t *cond);

/* Wake up all tasks that is wating on a condition variable */
PA_RET conc_cond_broadcast(conc_cond_t *cond);

/* Wait on a condition variable */
PA_RET conc_cond_wait(conc_cond_t *cond, conc_mutex_t *mutex);

/* Try to wait on a condition variable for an amount of time */
PA_RET conc_cond_timedwait(	conc_cond_t  *cond,  conc_mutex_t  *mutex,
							const  struct timespec *abstime);

/* Destroy a condition variable */
PA_RET conc_cond_destroy(conc_cond_t *cond);

#endif /* __conc_COND_H__ */

