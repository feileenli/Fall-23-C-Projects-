;;; Feileen Li
;;; 118924120
;;; fli1234

;;; Implementation that computes the nth fibonacci number.

    .global fib                 ; exposes fib as a label for the linker
fib:                            ; the label: the next instruction starts fib() function.

    tst r24         ; base case if n == 0
    breq 0f         ; if n = 0, branch to 0f

    cpi r24, 1      ; compare n = 1
    breq 1f         ;if n = 1, branch to 1f

    push r24        ; save n value
    push r25
    
    subi r24, 1     ; getting n - 1 value
    call fib        ; recursive call for fib(n-1)
    movw r30, r24   ;copy result to r30

    pop r25         ; restore it
    pop r24

    push r30	    ;save n-1 value
    push r31

    subi r24, 2     ; getting n - 2 value 
    call fib        ;recursive call

    pop r31
    pop r30
    
    add r24, r30    ;adding n-1 and n-2 
    adc r25, r31
    jmp 2f
0:
    ldi r24, 0      ;return 0
    ldi r25, 0
    jmp 2f

1:
    ldi r24, 1      ; return 1
    ldi r25, 0         

2:
    ret      

