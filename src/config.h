/* config.h

   A file that has configuration options used to build PAOS modules.
*/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/***************************************
 * Section ; Definition of Constants
 ***************************************/

/*
  Configuration of Memory Map
   - In case of Atmega128 without external RAM 
   - From Memory Footprint Configuration A of Atmega128 Datasheet Page 20
  
  +-------------------------------------+  <-- CONF_PAOS_SRAM_HIGH (0x10FF), CONF_MAIN_TASK_STACK_ADDR_HIGH
  | Main Task Stack ( 512 bytes )       |
  +-------------------------------------+  <--                               CONF_MAIN_TASK_STACK_ADDR_LOW
  |                                     |  <--                               CONF_PAOS_TASK_STACK_ADDR_HIGH
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  |                                     |
  +-------------------------------------+
  | Second Created Task ( Stack )       |
  +-------------------------------------+
  | First Created Task ( Stack )        |
  +-------------------------------------+  <-- CONF_PAOS_SRAM_LOW (0x0100) , CONF_PAOS_TASK_STACK_ADDR_LOW
  | External IO Regs, IO Regs, CPU Regs |
  +-------------------------------------+  <-- 0x0000
*/


/* The highest address of SRAM that PAOS can use */
#define CONF_PAOS_SRAM_HIGH (0x10FF)

/* The lowest address of SRAM that PAOS can use */
#define CONF_PAOS_SRAM_LOW (0x0400)

/* The default stack size of a task */
#define CONF_DEFAULT_TASK_STACK_SIZE (512)

/* The highest memory address of the main task can use */
#define CONF_MAIN_TASK_STACK_ADDR_HIGH ( CONF_PAOS_SRAM_HIGH )  

/* The lowest memory address of the main task can use */
#define CONF_MAIN_TASK_STACK_ADDR_LOW ( CONF_MAIN_TASK_STACK_ADDR_HIGH - CONF_DEFAULT_TASK_STACK_SIZE + 1 )

/* The highest memory address that PAOS can use as stack address */
#define CONF_PAOS_TASK_STACK_ADDR_HIGH  ( CONF_MAIN_TASK_STACK_ADDR_LOW -1 )

/* The lowest memory address that PAOS can use as stack address */
#define CONF_PAOS_TASK_STACK_ADDR_LOW ( CONF_PAOS_SRAM_LOW )

/* The default amount of timer tick for each task to own CPU 
   A task is gauranteed not to be scheduled for this number of timer ISR.
*/
#define CONF_DEFAULT_TASK_TICK (10)


/* The frequency of CPU clock */
#define F_CPU              (16000000UL)


/***************************************
 * Section ; UART
 ***************************************/

/* Baud rate for Serial communication */
#define UART_BAUD          (9600)

/* Transmission buffer size           */
#define UART_TXBUF_SIZE    (16)
/* Receive buffer size                */
#define UART_RXBUF_SIZE    (16)



/***************************************
 * Section ; Configuration of Modules
 ***************************************/

/* Enable Unit Test               */
/* #define PA_UTEST_ENABLE   (1)  */

/* Maxmum number of units to test */
/* #define PA_UTEST_MAXUNITS (16) */


/***************************************
 * Section ; Configuration of LCD Module
 ***************************************/

/* HD44780 LCD port connections */
#define HD44780_PORT A
#define HD44780_RS PORT6
#define HD44780_RW PORT4
#define HD44780_E  PORT5
#define HD44780_D4 PORT0
#define HD44780_D5 PORT1
#define HD44780_D6 PORT2
#define HD44780_D7 PORT3


/***************************************
 * Section ; Module Configuration
 ***************************************/
/* Enable or Disable Functions of Modules In Order To Reduce
   The Size of the Compiled Code */

//#define CONFIG_MUTEX_TIMEDLOCK    (1)   /* pthread_mutex_timedlock support? */
#define CONFIG_MUTEX_TRYLOCK      (1)   /* pthread_mutex_trylock support? */

#endif /* __CONFIG_H__ */
