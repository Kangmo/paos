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

#include <avr/io.h>

;
; Public Functions
;

    .global md_switch_context_by_task


; External Functions
    .extern sched_timer_tick
    .extern sched_next_task

; Load stack pointer of CurrTask( Current Task )
.macro load_curr_task_sp
    lds r30, CurrTask           ;   Z := address of CurrTask
    lds r31, CurrTask+1
    ld  r28, Z+                  ;   SP := *Z
    out _SFR_IO_ADDR(SPL), r28
    ld  r29, Z+
    out _SFR_IO_ADDR(SPH), r29
.endm


; Store stack pointer of CurrTask ( Current Task )
.macro store_curr_task_sp
    lds r30, CurrTask           ;   Z := address of CurrTask
    lds r31, CurrTask+1
    in  r28, _SFR_IO_ADDR(SPL)   ;   *Z = SP
    st  Z+, r28
    in  r29, _SFR_IO_ADDR(SPH)   ;
    st  Z+, r29
.endm


.macro push_reg_29_to_00
    push r29
    push r28
    push r27
    push r26
    push r25
    push r24
    push r23
    push r22
    push r21
    push r20
    push r19
    push r18
    push r17
    push r16
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push r7
    push r6
    push r5
    push r4
    push r3
    push r2
    push r1
    push r0
.endm

.macro pop_reg_00_to_29
    pop r0
    pop r1
    pop r2
    pop r3
    pop r4
    pop r5
    pop r6
    pop r7
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
    pop r16
    pop r17
    pop r18
    pop r19
    pop r20
    pop r21
    pop r22
    pop r23
    pop r24
    pop r25
    pop r26
    pop r27
    pop r28
    pop r29
.endm

; push SREG
.macro push_reg_status
    in r30, _SFR_IO_ADDR(SREG)
    push r30
.endm

; push SREG
;
; Set Interrupt Enable Flag if r31 == 1
; When Context Swiching is done by Timer ISR, we need to set
; Interrupt Enable Flag, because ATMEGA128 clears Interrupt Enable Flag in SREG
; Just before executing Interrupt Handler.
.macro push_reg_status_ie
    in r30, _SFR_IO_ADDR(SREG)
    cpi r31, 1     ; if r31 != 1
    brne push_r30  ;    goto push_r30
    sbr r30, 0x80  ; Global Interrupt Enable
push_r30:
    push r30
.endm

.macro pop_reg_status
    pop r31
    out _SFR_IO_ADDR(SREG), r31
.endm


.macro push_reg_rampz
    in r31, _SFR_IO_ADDR(RAMPZ)
    push r31
.endm

.macro pop_reg_rampz
    pop r31
    out _SFR_IO_ADDR(RAMPZ), r31
.endm

; For checking stack consistency - Push CA, FE
.macro push_stack_signature
    ldi r31, 0xCA
    push r31
    ldi r31, 0xFE
    push r31
.endm

; Check stack signature
.macro pop_stack_signature
    pop r31  ; 0xFE
    cpi r31, 0xFE                ; Check r31 == 0xFE
    brne stack_error_fe
    pop r31  ; 0xCA              ; Check r31 == 0xCA
    cpi r31, 0xCA
    brne stack_error_ca
    jmp  stack_ok
stack_error_fe:
    jmp stack_error_fe           ; halt CPU
stack_error_ca:
    jmp stack_error_ca           ; halt CPU
stack_ok :
.endm

    
.macro push_context_except_r31_r30
    push_reg_status_ie
    push_stack_signature
    push_reg_29_to_00
    push_reg_rampz
.endm

.macro pop_context_except_r31_r30
    pop_reg_rampz
    pop_reg_00_to_29
    pop_stack_signature
    pop_reg_status
.endm

   .data
    // nothing in eeprom section
    .section    .data
    // nothing in data section
   .text
    .section    .text


.global SIG_OVERFLOW0
SIG_OVERFLOW0:
TICK_ISR:
    // BUGBUG no need to push RAMPZ ?
    push r31               ; [1] push r31
    push r30               ; [2] push r30
    push_reg_status        ; [3] push SREG
    push_reg_29_to_00      ; [4] push registers

    ; Check if a new task should be executed right now.
    call sched_timer_tick  ;  Call C func. ( r24 := return value of type u8 )
    mov  r31, r24          ;    Save return value of sched_timer_tick to r31
                           ;    ( r24 is going to popped from stack )
                           ;        0 - no need to context_switch
                           ;        1 - context_switch is required

    pop_reg_00_to_29       ; [4] pop registers
    cpi r31, 0x00          ;     if (r31 != 0)
    brne do_context_switch_on_that_r31_r30_is_pushed ;  execute context switching
    
    pop_reg_status         ; [3] pop SREG
    pop r30                ; [2] pop r30
    pop r31                ; [1] pop r31
    reti                   ;     end of ISR

do_context_switch_on_that_r31_r30_is_pushed :
    pop_reg_status         ; [3] pop SREG
                           ; r30, r31 remains on stack

    ldi  r31, 1            ; Indicate to set Interrupt Enable Flag in SREG just before pushing SREG
    jmp md_switch_context_on_that_r31_r30_is_pushed  ;  do context switch
/*
    push r31
    push r30
    ldi  r31, 1            ; r31 := 1 ( Indicate to set Interrupt Enable Flag in SREG just before pushing SREG )
    jmp md_switch_context_on_that_r31_r30_is_pushed  ;  do context switch
*/
; Input Condition : CurrTask ( Current Running Task )
; Arguments
;    if r31 == 1 - push SREG with Interrupt Enable set
;    if r31 == 0 - push SREG without touching Interrupt Enable
;
; Side Effect     : CurrTask := Next Task To Run
md_switch_context_on_that_r31_r30_is_pushed :
    brid ie_disabled  ; hang if interrupt is enabled
ie_enabled_error :
    jmp ie_enabled_error
ie_disabled:
    // r31, r30 is already pushed onto stack
    // if r31 == 1 then set Interrupt Enable Flag in SREG
    push_context_except_r31_r30
    store_curr_task_sp
    call sched_next_task     ; CurrTask := Next Task To Run
    load_curr_task_sp
    pop_context_except_r31_r30
    pop r30
    pop r31
    ret                      ; not reti but ret.

; Called by currently running task to execute context switching voluntarily
; SREG should not be touched while pushing Tasks context onto stack.
; Side Effect     : CurrTask := Next Task To Run
md_switch_context_by_task:
    push r31
    push r30
    ldi  r31, 0   ; r31 := 0  <== To indicate not to set Interrupt Enable Flag in SREG just before pushing it.
    jmp md_switch_context_on_that_r31_r30_is_pushed  ;  do context switch

