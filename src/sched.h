#ifndef __SCHED_H__
#define __SCHED_H__ (1)

#include "task.h"

/* Initialize the Scheduler

   Note that paos_task_stack does not include address of main_task_stack
*/
void sched_init(md_stackp_t paos_task_stack_low,  /* lowest address of task stack       */
                md_stackp_t paos_task_stack_high, /* highest address of task stack      */
                md_stackp_t main_task_stack_low,  /* lowest address of main task stack  */
                md_stackp_t main_task_stack_high);/* highest address of main task stack */


/* Perform a voluntary context switching */
void sched_task_yield( void );

/* Add a Task to run */
void sched_task_add(task_t * task);


/* Make a Task runnable and make it run at next context switching */
void sched_make_task_runnable(task_t *waiting_task);

/* Make all Tasks in the list runnable
   and make one of them run at next context switching */
void sched_make_tasks_runnable(list_t * waiting_tasks);

/* Remove the current Task from the Run-queue */
void sched_make_current_task_not_runnable( void );

/* Wait for a Task to complete its job */
PA_RET sched_task_join(task_t * task, task_ret_t * task_return );

/* Create a Task */
PA_RET sched_task_create(task_t * task, task_func_t task_func, task_arg_t task_arg,
                         task_stack_size_t task_stack_size );


/* Currently Running Task */
extern task_t * CurrTask ;

/* Get the currently running Task */
#define sched_current_task() (CurrTask)

#endif


