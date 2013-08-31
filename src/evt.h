/*  evt.h 

    The Event Handling System used by Tasks

	- What is an event ?
	  - The event can be anything that Task waits for.
	  - Examples
	    - sleep(10)
		  - Task waits for an alarm event that occurrs after 10 seconds ( The alaram event )
		- pthread_mutex_lock
		  - Task waits for the mutex owner to release the mutex. ( The mutex release event )
		- pthread_cond_wait
		  - Task waits for a cond to be signaled ( The signal event )
		- pthread_cond_timedwait
		  - Task waits for either a cond to be signaled or an alarm event occurrs
		    ( The signal event or The alarm event )
*/

#ifndef __EVT_H__
#define __EVT_H__ (1)

#include "list.h"
#include "task.h"

/* An Event 
   
   An event maintains a list of tasks waiting on it 
*/
typedef struct evt_t {
	list_t waiting_list;  /* list of tasks waiting on this event */
} evt_t;

/* Initialize an Event */
void evt_init( evt_t * evt );

/* Make a task to wait for an Event releasing CPU */
void evt_wait( evt_t * evt, task_t * task);

/* Wakeup an arbitrary task that is waiting on an Event */
void evt_wakeup( evt_t * evt );

/* Wakeup all tasks that are waiting for an Event */
void evt_wakeup_all( evt_t * evt );

/* Destroy an Event */
#define evt_destroy(evt) // do nothing


#endif /* __EVT_H__ */



