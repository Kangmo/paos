/* base.h

   Base layer is the lowest layer of PAOS, among layers 
   which are independent of hardware specific codes.

   - Main Routines
     - Locking whole PAOS( base_paos_lock, base_paos_unlock )
       - To ensure the consistency, while modifying internal structure of PAOS,
         No context switch should occurr. 

       - base_paos_lock disables interrupt including timer interrupt, which
         initiates preemptive context switching

       - base_paos_unlock enables interrupt
         only when it is called exactly same times as base_paos_lock

       - Each time base_paos_lock is called, base_paos_unlock should be called also.
*/

#ifndef __BASE_H__
#define __BASE_H__ (1)

#include "md.h"
#include "md_types.h"

/* The OS Lock Context  

   A task can voluntarily perform context switching 
   while the lock is acquired.

   That means each task has its own lock context,
   which should be saved as a part of task context 
   when context switching is performed.

*/
typedef struct base_lock_context_t {
	u8 lock_count;                   /* Numer of times base_paos_lock() is called */
} base_lock_context_t ;


/* The OS lock context of the currently running task */
extern base_lock_context_t PAOSLockCtx;

/* Lock PAOS scheduler */
void base_paos_lock( void );


/* Unlock PAOS scheduler */
void base_paos_unlock( void );


/* Initialize Lock Context */
PA_INLINE void base_paos_init_lock_context( base_lock_context_t * lock_ctx ) {
    lock_ctx->lock_count = 0;
}

/* Get the Lock Context */
PA_INLINE base_lock_context_t base_paos_get_lock_context( void ) {
	return PAOSLockCtx;
}

/* Set the Lock Context */
PA_INLINE void base_paos_set_lock_context(base_lock_context_t lock_ctx) {
	PAOSLockCtx = lock_ctx;
}


// Check is PAOS is locked
// Use of this function should be limited only to assertion checking
PA_INLINE bool_t base_paos_islocked( void ) {
    return ( PAOSLockCtx.lock_count > 0 );
}

#endif



