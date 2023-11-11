; This is a small demo to show the context switching that can be done between
; two tasks. Each of the two tasks shows a message and then runs a soft
; interrupt that switches the context to the other task. One of the task updates
; a counter and exits the program after it reaches a certain value.

;--------------------------------- definitions --------------------------------;

@define-math stack_size 1024
@define-math task_1_stack 0x8000
@define-math task_2_stack task_1_stack+stack_size

@define-math task_1_store task_2_stack+stack_size
@define-math task_2_store task_1_store+64

@define context_saving_register R2
@define loop_count_register     R3
@define used_interrupt          3

;----------------------------- two tasks routines -----------------------------;

; The first task prints its message and check the loop counter if it must exits.
label routine_1
    set 3
    leseq loop_count_register
    jifl quit
    inc. loop_count_register
    prints "Hi from routine 1\n"
    softint used_interrupt
    goto routine_1
label quit
    quit

; The second task prints its message and put some garbage in the loop counter
; register to shows that it is well saved in the context of the other task.
label routine_2
    prints "Hello from routine 2\n"
    softint used_interrupt
    set 0xF
    cpy loop_count_register
    goto routine_2

;------------------------------ context switching -----------------------------;

; This routine should be used as the ISR. It first save the context of the
; current task, saves it in the appropriate memory space (task_X_store) and
; fetches the context of the other task.
label int_trig
    ros_save_context context_saving_register used_interrupt
    setlab int_restore
    setintstack used_interrupt
    ; swap stack1 and stack2
    set+ stack_size
    and SP
    cpy R8
    set 0
    eq R8
    cmpnot
    jifl is_in_stack_2
    label is_in_stack_1
        ; if here, we are in stack 1
        setr R1 task_1_store
        read context_saving_register
        str R1
        set+ task_2_store
        cpy R1
        load R1
        cpy context_saving_register
        retint
    label is_in_stack_2
        ; if here, we are in stack 2
        setr R1 task_2_store
        read context_saving_register
        str R1
        setr R1 task_1_store
        load R1
        cpy context_saving_register
        retint

; This is a small function that restores the context of a task. It must be used
; as the function to return to from the interrupt context.
label int_restore
    ros_restore_context context_saving_register

;---------------------------- state initialization ----------------------------;

; This macro sets the stack as if we just saved the context from the task 2.
; As the tasks are not launched for real in this demo, we must do it that way.
@macro fake_task_2_ctx 0
    setlab routine_2
    push
    push
    pushr. R1
    pushr. R2
    pushr. R3
    pushr. R4
    pushr. R5
    pushr. R6
    pushr. R7
    pushr. R8
    pushr. R9
    pushr. R10
    pushr. R11
    pushr. R12
    pushr. SR
@end 

label start
    ; Enable interrupt.
    mov. SP R1
    setlab int_trig
    setint used_interrupt
    set8 0xFF
    or SR
    cpy SR

    ; Prepare the state of the task 2.
    setr R2 task_2_store
    setr SP task_2_stack
    fake_task_2_ctx
    read SP
    str R2

    ; Prepare the state of the task 1 and run it.
    set 0
    cpy loop_count_register
    set+ task_1_stack
    cpy SP
    goto routine_1

;----------------------------------- imports ----------------------------------;

@import ../libs/import_all_libs.asm
@import reflet_os.asm

