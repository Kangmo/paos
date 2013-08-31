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
         
          Dual License
            1. GPL
            2. PAOS Commercial License
             
          License Selection Algorithm             
            IF You can open your whole source code
              You are under GPL license
            ELSE
              You are under PAOS Commercial License
            FI
                                
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

#include "md.h"
#include "base.h"
#include "task.h"
#include "sched.h"

md_stackp_t TaskStackAddrLow;   /* = 0,  Lowest  address of memory used for the stack of tasks */
md_stackp_t TaskStackAddrHigh;  /* = 0,  Highest address of memory used for the stack of tasks */

list_t ToRunTaskList;      /* Run-queue : list of Tasks waiting for CPU */
list_t AlreadyRanTaskList; /* list of Tasks already ran on CPU */

/* 
    The scheduler maintains two Task lists.
    One is list of tasks that is waiting for CPU, 
    and the another is list of tasks that already got the CPU.

    The first list ToRunTasks, and the second one is AlreadyRanTasks.

    The scheduler starts to remove a task from ToRunTask
    each time it should choose a new Task to be scheduled.
     
    After all ToRunTasks are ran, AlreadyRanTasks and ToRunTasks is switched 
    and Tasks that were in AlreadyRanTasks begins to run. 
*/
list_t * ToRunTasks      = & ToRunTaskList;      /* Holds tasks to run */
list_t * AlreadyRanTasks = & AlreadyRanTaskList; /* Holds tasks already ran, (or currently running) */


task_t * CurrTask; /* = NULL, The currently running Task */

task_t MainTask;   /* The main Task which has main() function */

/* Initialize the Scheduler

   Note that paos_task_stack does not include address of main_task_stack
*/
void sched_init(md_stackp_t paos_task_stack_low,
                md_stackp_t paos_task_stack_high,
                md_stackp_t main_task_stack_low,
                md_stackp_t main_task_stack_high) 
{
    TaskStackAddrLow  = paos_task_stack_low;
    TaskStackAddrHigh = paos_task_stack_high;
    
    /* Initialize the Main Task
       The structure of the Main Task is initialized at the first context switch */
    task_init_main( & MainTask, main_task_stack_low, main_task_stack_high);

    /* Initialize Task lists */
    list_init( ToRunTasks );
    list_init( AlreadyRanTasks );

    /* Because the Main task is already running, add it to AlreadyRanTasks */
    list_add_head( AlreadyRanTasks, & MainTask.task_node );

    /* The currently running task is the Main Task */
    CurrTask  = & MainTask;

    /* Start scheduling tasks */
    md_start_timer();
}

/* Create a Task */
PA_RET sched_task_create(task_t * task, task_func_t task_func, task_arg_t task_arg,
                             task_stack_size_t task_stack_size )
{
    PA_RET ret = 0;
    
    base_paos_lock();

    md_stackp_t task_stack_low  = TaskStackAddrLow + 1;
    md_stackp_t task_stack_high = TaskStackAddrLow + task_stack_size;

    /* Out of task stack memory */
    if ( task_stack_high > TaskStackAddrHigh ) {
        ret = -1;
        goto unlock_and_return;
    }

    /* Initialize Task */
    (void) task_init(   task,
                        task_func,
                        task_arg,
                        task_stack_low,      /* Low Address  of the Stack */
                        task_stack_high );   /* High Address of the Stack */

    /* Add task to scheduler */
    (void) sched_task_add( task );

    /* Update lowest address of task stack for the next time sched_task_create is called */
    TaskStackAddrLow = task_stack_high;
    

unlock_and_return:
    base_paos_unlock();

    return ret;
}

/* Wait for a Task to complete its job */
PA_RET sched_task_join(task_t * task, task_ret_t * task_return ) 
{
    // needs to be implemented
    return -1;
}


/* Perform a voluntary context switching */
void sched_task_yield( void )
{
    /* prohibit timer ISR handler from being executed while context switching is in progress */
    base_paos_lock();   

    md_switch_context_by_task();

    base_paos_unlock();   
}


/* Add a Task to run */
void sched_task_add(task_t * task)
{
    base_paos_lock();

    /* Append the Task to the end of the list of the tasks that is going to be run */
    list_add_tail( ToRunTasks, & task->task_node );

    base_paos_unlock();
}

/*
    Make a Task runnable and make it run at next context switching
    
    Precondition : The Task was waiting for an event to happen.
                   ( The Task was not a ready to run, running, nor already ran Task )
 */
void sched_make_task_runnable(task_t *waiting_task)
{
    base_paos_lock();
    
    /* move task to the first node of ToRunTasks */
    list_add_head( ToRunTasks, & waiting_task->task_node );

    base_paos_unlock();
}


/*
    Make all Tasks in the list runnable
    and make one of them run at next context switching.

    Precondition : The tasks were waiting for an event to happen.
                   ( They were not ready to run, running, nor already ran )
 */
void sched_make_tasks_runnable(list_t * waiting_tasks)
{
    base_paos_lock();
    
    // move task to the first node of ToRunTasks
    list_append_to_head( ToRunTasks, waiting_tasks );

    base_paos_unlock();
}

/*
    Remove the current Task from the Run-queue

    Note: The current Task went into AlreadyRanTasks 
          by the time the Task was selected to be executed by sched_next_task
*/
void sched_make_current_task_not_runnable( void )
{
    base_paos_lock();

    list_remove( AlreadyRanTasks, & CurrTask->task_node );

    base_paos_unlock();
}




/*
    A callback function that is called on each timer tick by timer ISR at md_asm.S

    Determine whether context swiching should be performed or not.
*/
u8 sched_timer_tick( void )
{
    u8 need_contex_switch = 0;

    PA_ASSERT( CurrTask->task_tick > 0 );

    CurrTask->task_tick --;

    /* Ran out of task tick ? */
    if ( CurrTask->task_tick == 0 )
    {
        need_contex_switch = 1;
        /* refill task tick */
        CurrTask->task_tick = CONF_DEFAULT_TASK_TICK;
    }

    return need_contex_switch;
}


/*
    Determine the NextTask to run

    A callback function that is called either by timer ISR or voluntarily by a Task.
    ( called from md_asm.S )
    
    [ Getting/Setting the lock context. ]

    PAOS scheduling system supports Task's voluntary context switching 
    while base_paos_lock() is invoked but base_paos_unlock() is not invoked yet.
    When context switched, the lock context that has the number of times base_paos_lock() is called 
    is sved task control block( task_t structure ).

    Next time the task starts running, the lock count in the lock context is restored.
*/
void sched_next_task( void )
{
    list_t      * tmp;
    list_node_t * task_node_to_run;

    /* static int tick = 0;
      _DEBUG_( PA_TRACE( TM_SCHED, TL_FUNC,"[%d] In sched_next_task\n\r", ++tick ) ); */

    PA_ASSERT( task_stack_ok( CurrTask ) );
 
    /* No more task in ToRunTasks ? */
    if ( list_isempty( ToRunTasks ) )
    {
        /* Start to run tasks in AlreadyRunTasks,
           by swapping ToRunTasks and AlreadyRunTasks */
        tmp = ToRunTasks;
        ToRunTasks = AlreadyRanTasks;
        AlreadyRanTasks = tmp;
    }
    
    /* ToRunTasks should not be empty */
    PA_ASSERT( ! list_isempty( ToRunTasks ) );

    task_node_to_run = (list_node_t*) ToRunTasks->head;

    /* move task node from ToRunTasks to AlreadyRanTasks */
    list_remove( ToRunTasks, task_node_to_run );
    list_add_tail( AlreadyRanTasks, task_node_to_run );

    /* save the lock context of the current task
       ( ex> how man times locks */
    task_set_lock_context( CurrTask, base_paos_get_lock_context() );

    /* set the new task to run */
    CurrTask = (task_t*) task_node_to_run->data;

    /* restore the lock context of the new task */
    base_paos_set_lock_context( task_get_lock_context( CurrTask ) );

    PA_ASSERT( task_stack_ok( CurrTask ) );
}
