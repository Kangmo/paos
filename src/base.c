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

/* The OS Lock Context */
base_lock_context_t PAOSLockCtx ; // = { lock_count=0 };


/* Lock PAOS scheduler

   In order to provide consistency of kernel data structures,
   a task which modifies them should lock the scheduler.

   When the scheduler is locked, the scheduler suspends 
   context switching until the scheduler is unlocked again.

   Possible Enhancement : 
      It is not a good idea to disable Interrupts 
	  whenever the scheduler is locked.
	  
	  The timer interrupt service routine has resposibility
	  to wake up sleeping tasks, which is very important 
	  in realtime operating systems.

	  By using a flag such as IsSchedulerLocked, base_paos_lock
	  can be modified to set the flag to true.
	  
	  In the timer interrupt service routine, the same flag can
	  be checked if the scheduler can perform context switching. 

	  Note that changing IsSchedulerLocked is not atomic, 
	  interrupts should be disabled while the flag is being set. 
*/
void base_paos_lock( void ) {
    md_int_disable();
    PA_ASSERT( PAOSLockCtx.lock_count < U8_MAX );
    PAOSLockCtx.lock_count ++;
}


/* Unlock PAOS scheduler */
void base_paos_unlock( void ) {
    if ( --PAOSLockCtx.lock_count == 0 ) {
        md_int_enable();
    }
}
