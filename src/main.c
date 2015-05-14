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
#include "pthread.h"

void* thread_basic_1_routine(void* arg)
{
    arg = arg; // to prevent compiler warning
    
    int i=0;

    while(1)
    {

        if (i % 10000 == 0 )
            _DEBUG_( PA_TRACE( TM_SCHED, TL_FUNC,"[%d] In task_A\n\r", i));

        i+=2;
    }
    return 0;
}

void* thread_basic_2_routine(void* arg)
{
    arg = arg; // to prevent compiler warning

    int i=0;
    while(1)
    {

        if (i % 10000 == 5000 )
            _DEBUG_( PA_TRACE( TM_SCHED, TL_FUNC,"[%d] In task_B\n\r", i));
        i+=2;
    }

    return 0;
}

void test_thread_basic( void )
{
    pthread_t thread_1;
    pthread_t thread_2;
    void    * thread_ret;

    pthread_create( & thread_1, NULL, & thread_basic_1_routine, (void*) 0 );
    pthread_create( & thread_2, NULL, & thread_basic_2_routine, (void*) 0 );

    while(1) {
        pthread_yield();
    }
    pthread_join( thread_1, & thread_ret );
    pthread_join( thread_2, & thread_ret );
}


pthread_mutex_t Mutex;
bool_t          IsLocked;

void* thread_mutex_1_routine(void* arg)
{
    arg = arg; // to prevent compiler warning
    

    while(1)
    {
        if ( pthread_mutex_lock(&Mutex) != 0 )
            PA_ASSERT(0);

            PA_ASSERT( ! IsLocked );

            IsLocked = 1;

            PA_TRACE( TM_SCHED, TL_FUNC,"In task_1\n\r" );
            //md_delay_ms(100);

            IsLocked = 0;

        if ( pthread_mutex_unlock(&Mutex) != 0 )
            PA_ASSERT(0);
    }
    return 0;
}

void* thread_mutex_2_routine(void* arg)
{
    arg = arg; // to prevent compiler warning

    while(1)
    {
        if ( pthread_mutex_lock(&Mutex) != 0 )
            PA_ASSERT(0);

            PA_ASSERT( ! IsLocked );

            IsLocked = 1;

            PA_TRACE( TM_SCHED, TL_FUNC,"In task_2\n\r" );
//          md_delay_ms(100);

            IsLocked = 0;

        if ( pthread_mutex_unlock(&Mutex) != 0 )
            PA_ASSERT(0);
    }

    return 0;
}




void test_thread_mutex( void )
{
    pthread_t thread_1;
    pthread_t thread_2;
    void    * thread_ret;
//  u8        main_thread_loops;

    if ( pthread_mutex_init( &Mutex, NULL/* Default Attribute*/ ) != 0 )
        PA_ASSERT(0);

    if ( pthread_create( & thread_1, NULL, & thread_mutex_1_routine, (void*) 0 ) != 0 )
        PA_ASSERT(0);

    if ( pthread_create( & thread_2, NULL, & thread_mutex_2_routine, (void*) 0 ) != 0 )
        PA_ASSERT(0);

    while(1) {  
//  while( main_thread_loops++ < 10 ) {
        if ( pthread_mutex_lock(&Mutex) != 0 )
            PA_ASSERT(0);

        PA_ASSERT( ! IsLocked );

        PA_TRACE( TM_SCHED, TL_FUNC,"In task_main\n\r" );
    
//      md_delay_ms(100);

        if ( pthread_mutex_unlock(&Mutex) != 0 )
            PA_ASSERT(0);
        
        if ( pthread_yield() != 0 )
            PA_ASSERT(0);
    }

    pthread_join( thread_1, & thread_ret );
    pthread_join( thread_2, & thread_ret );


    if ( pthread_mutex_destroy( &Mutex ) != 0 )
        PA_ASSERT(0);

}

int main( void )
{

    _DEBUG_( trace_init(&td_serial) );
//    _DEBUG_( trace_init(&td_lcd) );
    _DEBUG_( PA_TRACE( TM_SCHED, TL_FUNC,"Start of main\n\r") );

    // test_thread_basic();
    test_thread_mutex();
    return 0;
}

