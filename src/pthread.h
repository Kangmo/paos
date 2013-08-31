/*
	pthread.h

	The pthread library interfaces
*/

#ifndef __PTHREAD_H__
#define __PTHREAD_H__ (1)



#include "config.h"
#include "conc_cond.h"
#include "conc_mutex.h"
#include "sched.h"

#define PTHREAD_MUTEX_INITIALIZER TASK_MUTEX_INITIALIZER
#define PTHREAD_COND_INITIALIZER TASK_COND_INITIALIZER


/* initialize pthread system */
PA_INLINE void pthread_system_init( void )
{
    sched_init( (md_stackp_t) CONF_PAOS_TASK_STACK_ADDR_LOW, (md_stackp_t) CONF_PAOS_TASK_STACK_ADDR_HIGH,
                (md_stackp_t) CONF_MAIN_TASK_STACK_ADDR_LOW, (md_stackp_t) CONF_MAIN_TASK_STACK_ADDR_HIGH );
}


/*
    pthread_attr_t
*/
typedef struct pthread_attr_t {
    u16 thread_stack_size;     /* The size of the Task stack */
} pthread_attr_t;

#define pthread_attr_init(ta)                (ta)->ta_stack_size = CONF_DEFAULT_STACK_SIZE;
#define pthread_attr_getstacksize(ta,size)   size = (ta)->ta_stack_size;
#define pthread_attr_setstacksize(ta,size)   (ta)->ta_stack_size = size;
#define pthread_attr_destroy(ta)             /* Do Nothing */


/* implementation specific type conc_mutex_t is hidden to pthread API */
typedef struct pthread_t {
    task_t __do_not_access_this_field__;
} pthread_t ;


int pthread_create( pthread_t  * thread,  pthread_attr_t * attr, void *(*start_routine)(void *),
                    void * arg);

pthread_t pthread_self( void );

PA_INLINE int pthread_yield( void ) {
	(void) sched_task_yield();
	return 0;
}

PA_INLINE int pthread_join(pthread_t th, void **thread_return) {
    return sched_task_join( (task_t*) & th, thread_return );
}


/*
    pthread_mutex_t
*/

/* implementation specific type conc_mutex_t is hidden to pthread API */
typedef struct pthread_mutex_t {
    conc_mutex_t __do_not_access_this_field__;
} pthread_mutex_t ;

/* Unused */
typedef struct pthrad_mutexattr_t {
} pthread_mutexattr_t;



struct conc_mutex_t;

PA_INLINE int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr) {
	mutexattr = mutexattr; // to prohibit compiler warning
	return conc_mutex_init( (conc_mutex_t *)mutex );
}

PA_INLINE int pthread_mutex_lock(pthread_mutex_t *mutex) {
	return conc_mutex_lock( (conc_mutex_t *)mutex );
}

#if defined ( CONFIG_MUTEX_TIMEDLOCK )
PA_INLINE int pthread_mutex_timedlock(pthread_mutex_t * mutex,
                            const struct timespec * abs_timeout) {
    return conc_mutex_timedlock( (conc_mutex_t *) mutex, abs_timeout );
}
#endif /* CONFIG_MUTEX_TIMEDLOCK */

#if defined ( CONFIG_MUTEX_TRYLOCK )
PA_INLINE int pthread_mutex_trylock(pthread_mutex_t *mutex){
	return conc_mutex_trylock( (conc_mutex_t *)mutex );
}
#endif /* CONFIG_MUTEX_TRYLOCK */

PA_INLINE int pthread_mutex_unlock(pthread_mutex_t *mutex) {
	return conc_mutex_unlock( (conc_mutex_t *)mutex );
}

PA_INLINE int pthread_mutex_destroy(pthread_mutex_t *mutex) {
	return conc_mutex_destroy( (conc_mutex_t *)mutex );
}


/*
    pthread_cont_t
*/
typedef struct pthread_cond_t {
    conc_cond_t __do_not_access_this_field__;
} pthread_cond_t ;

/* pthread_condattr_t is unsupported yet */
typedef void * pthread_condattr_t;

PA_INLINE int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *condattr ) {
    condattr = condattr ;  // to prohibit compiler warning
	return conc_cond_init( (conc_cond_t *) cond );
}

PA_INLINE int pthread_cond_signal(pthread_cond_t *cond) {
	return conc_cond_signal( (conc_cond_t *) cond );
}

PA_INLINE int pthread_cond_broadcast(pthread_cond_t *cond) {
	return conc_cond_broadcast( (conc_cond_t *) cond );
}

PA_INLINE int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
	return conc_cond_wait( (conc_cond_t *) cond, (conc_mutex_t *) mutex );
}

PA_INLINE int pthread_cond_timedwait ( pthread_cond_t  * cond,
   									pthread_mutex_t * mutex,
    								const struct timespec *abstime ) {
    return conc_cond_timedwait( (conc_cond_t *) cond, (conc_mutex_t *)mutex, abstime );
}

PA_INLINE int pthread_cond_destroy(pthread_cond_t *cond) {
    return conc_cond_destroy( (conc_cond_t *) cond );
}


#endif /* __PTHREAD_H__ */




