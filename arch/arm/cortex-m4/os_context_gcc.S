.syntax unified
.cpu cortex-m4
.thumb

.global PendSV_Handler
.global os_scheduler_start

.extern os_curr_task
.extern os_next_task

/* Start the first task */
.section .text
.thumb
.type os_scheduler_start, %function
.thumb_func
os_scheduler_start:
@ 1) Set os_curr_task = NULL to indicate first context switch
    mov r0, #0
    ldr r1, =os_curr_task
    str r0, [r1]

@ 2) Trigger PendSV by setting bit 28 of ICSR
    ldr r0, =0xE000ED04
    ldr r1, =0x10000000
    str r1, [r0]

@ 3) Enable interrupts so PendSV can run
    cpsie i
    dsb
    isb

hang:
    b hang  @ Hang here until PendSV runs and switches to first task


/* Context switch (PendSV_Handler) */
.type PendSV_Handler, %function
.thumb_func
PendSV_Handler:
    cpsid i                 @ Disable interrupts

    ldr r0, =os_curr_task
    ldr r1, [r0]
    cbz r1, switch_to_next  @ If curr == NULL, skip saving context (first switch)

    mrs r2, psp             @ Read current PSP
    stmdb r2!, {r4-r11}     @ Save R4-R11
    str r2, [r1, #0]        @ Store SP back into TCB (SP at offset 0)

switch_to_next:
    ldr r0, =os_next_task
    ldr r1, [r0]
    ldr r2, =os_curr_task
    str r1, [r2]            @ curr = next

    ldr r0, [r1, #0]        @ Load SP from new task TCB (offset 0)
    ldmia r0!, {r4-r11}     @ Restore new task R4-R11
    msr psp, r0             @ Update PSP register

    cpsie i                 @ Re-enable interrupts
    orr lr, lr, #0x04       @ Ensure exception return uses PSP
    bx lr
