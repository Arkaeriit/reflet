@import ../libs/import_all_libs.asm
@import reflet_os.asm

@define-math ctx_stack_size 1024
@define-math task_1_stack 0x8000
@define-math task_2_stack task_1_stack+ctx_stack_size

@define-math task_1_store task_2_stack+ctx_stack_size
@define-math task_2_store task_1_store+64

@define context_saving_register R2
@define loop_count_register     R3

label routine_1
    set 3
    leseq loop_count_register
    jifl quit
    inc. loop_count_register
    prints "Hi from routine 1\n"
    softint 3
    goto routine_1

label routine_2
    prints "Hello from routine 2\n"
    softint 3
    goto routine_2

label int_trig
    ros_save_context context_saving_register 3
    setlab int_restore
    setintstack 3
    ; swap stack1 and stack2
    set+ ctx_stack_size
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
        setr R1 task_2_store
        read context_saving_register
        str R1
        setr R1 task_1_store
        load R1
        cpy context_saving_register
        retint


label int_restore
    ros_restore_context context_saving_register

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
    mov. SP R1
    setlab int_trig
    setint 3
    set8 0xFF
    or SR
    cpy SR

    setr R1 task_1_store
    cpy R1
    set+ task_1_stack
    str R1
    cpy R1

    setr R2 task_2_store
    setr SP task_2_stack
    fake_task_2_ctx
    read SP
    str R2

    set 0
    cpy loop_count_register
    mov. SP R1
    goto routine_1

label quit
    quit

