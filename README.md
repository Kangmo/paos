PAOS (Peter-Anna Operating System)
==================================

An Embedded OS kernel with pthread API running on Atmel AVR

Summary
=======
PAOS implements pthread interface on Atmel AVR such as [ATmega128](http://www.atmel.com/devices/atmega128.aspx)

List of Features
================

*  Preemptive scheduler @ pthread interface
*  USART driver

Current Project Status 
======================

Feature and ( Implementation progress / Testing progress )

*  pthread_create ... 90% / 50%
*  pthread_self ... 90% / 50%
*  pthread_join ... 0% / 0%
*  pthread_mutex_init ... 90% / 50%
*  pthread_mutex_lock ... 90% / 50%
*  pthread_mutex_trylock ... 90% / 50%
*  pthread_mutex_unlock ... 90% / 50%
*  pthread_mutex_destroy ... 90% / 50%
*  pthread_attr_init ... 90% / 0%
*  pthread_attr_getstacksize ... 90% / 0%
*  pthread_attr_setstacksize ... 90% / 0%
*  pthread_attr_destroy ... 90% / 0%
*  pthread_cond_init ... 0% / 0%
*  pthread_cond_signal ... 0% / 0%
*  pthread_cond_broadcast ... 0% / 0%
*  pthread_cond_wait ... 0% / 0%
*  pthread_cond_timedwait ... 0% / 0%
*  pthread_cond_destroy ... 0% / 0%

License
=======
GPL v2 License

