/* task.h
 
   Maintains the data structure of a Task
*/
#ifndef __TASK_H__
#define __TASK_H__ (1)

#include "md.h"
#include "base.h"
#include "list.h"

typedef void * task_arg_t;
typedef void * task_ret_t;

typedef u16    task_stack_size_t;

/*
    The task function type 

    Note : task_func_t should be exactly same type to
           start_routine argument of pthread_create interface
*/
typedef task_ret_t (*task_func_t)(task_arg_t);

/*  The task structure  */
typedef struct task_t
{	
	/* The stack pointer of the Task */
	md_stackp_t     task_SP;

    /* The highest memory of the Task stack */
    md_stackp_t     task_stack_high;

    /* The lowest memory of the Task stack */
    md_stackp_t     task_stack_low;

	/* The main function of the Task */
	task_func_t     task_func;

	/* The argument that should be passed to the main function */
	task_arg_t      task_arg;

    /* The exit code of the Task ( The return value by the function typed task_func_t ) */
	task_ret_t      exit_code;

	/* The duration that the Task has CPU ( in number ISR ticks )
       Task is not context switched for this time tick */
	u16             task_tick;

	/* list_node_t pointing to the Task itself */
	list_node_t     task_node;

	/* The lock context of the Task. ( ex> How many times base_paos_lock is invoked? ) */
	base_lock_context_t lock_context;
} task_t ;



/* Initialize a Task */
void task_init( task_t * task, task_func_t task_func, task_arg_t task_arg, md_stackp_t stack_low, md_stackp_t stack_high);

/* Initialize the Main Task */
void task_init_main( task_t * task, md_stackp_t stack_low, md_stackp_t stack_high);

/* Check stack overrun or underrun */
bool_t task_stack_ok( task_t * task );

/* Set the lock context of a task */
PA_INLINE void task_set_lock_context( task_t * task, base_lock_context_t lock_ctx )
{
	task->lock_context = lock_ctx;
}

/* Get the lock context of a task */
PA_INLINE base_lock_context_t task_get_lock_context( task_t * task )
{
	return task->lock_context;
}

#endif /* __TASK_H__ */


