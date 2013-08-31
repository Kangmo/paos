/*  debug.h

    Debugging facility for tracing and asserting
*/
#ifndef __DEBUG_H__
#define __DEBUG_H__ (1)

#include "md_types.h"

/* define PA_DEBUG to enable assertions, trace messages */
#define PA_DEBUG (1)

/* The trace driver

   While tracing, a trace message is shown to the user. 
   Trace message is usually a string that human can read.
   
   The trace driver prints each character in trace mesage strings
   Typical trace drivers are LCD, USART.
*/
typedef struct trace_driver_t {
	void (*init)(void);          /* initialize the driver */
	void (*putchar)(u8 ch);      /* print a character     */
} trace_driver_t;

/* The module that printed a trace message */
typedef enum trace_module_t {
	TM_SCHED = 1,
	TM_TEST
} trace_module_t ;

/* The trace level of a trace message */
typedef enum trace_level_t {
	TL_FUNC = 1
} trace_level_t ;

/* exported trace drivers */
extern trace_driver_t td_serial; /* Trace message is sent to Serial Communication */
extern trace_driver_t td_lcd;    /* Trace message is sent to LCD */

/* initialize the tracing system with a trace driver */
void trace_init(trace_driver_t * driver);

/* Print a trace message ( printf like version ) */
void tracef(trace_module_t module, trace_level_t level, char* format, ... )
     __attribute__ ((format (printf, 3, 4)));  /* check consistency of printf style arguments */

/*  Print an assertion expression, a source file, a line number 
    where the assertion failure has occurred
*/
void trace_assertion( s8 * exp, s8 * file, u16 line );

#if defined (PA_DEBUG)

	#define PA_TRACE tracef

	#define PA_HALT() { md_int_disable(); while(1); }

	#define PA_ASSERT( exp ) {                                    \
		if ( ! (exp) ) {                                          \
			trace_assertion( (s8*)#exp, (s8*)__FILE__, __LINE__); \
			PA_HALT();                                            \
		}                                                         \
	}

	#define _DEBUG_(x) (x);

#else  /* ! PA_DEBUG */

	#define PA_HALT() 

	#define PA_ASSERT( exp ) 

	#define _DEBUG_(x)

#endif /* PA_DEBUG */

#endif /* __DEBUG_H__ */
