;;; Feileen Li
;;; 118924120
;;; fli1234

;;; determines if word is a palindrome 

    .global is_palindrome

is_palindrome:

    push r14        ;callee saved
    push r15
    push r16

    movw r26, r24   ;copy argument to x pointer
    push r26        ; save x pointer
    push r27
    movw r30, r24   ;copy argument to z pointer 

    call Strlen     ;changes values in r25 and r27
    movw r14, r24   ;copy strlen into r15
    add r30, r14    ;add length to z pointer 
    dec r30         ;dec z pointer by 1
    lsr r14         ;divide strlen by 2
    ldi r16, 0      ;loop starts, r16 iteration variable
    clr r24
    clr r25
    pop r27
    pop r26

loop:
    cp r16, r14     ;checking whether we reach limit
    brsh 1f         ;branch to 1 if we reach end of string
   
    ld r24, X+      ; reading value and moving X
    clr r20
    clr r21
    ld r20, Z       ;reading value of Z pointer
    subi r30, 1      ;decrement z pointer by 1
    sbc r31, 0
    cp r24, r20     ;compare characters 
    breq equal      ; branch if characters are equal

    ldi r24, 0      ; else set return arg to 0
    jmp end

equal:
    inc r16         ;increase tracker
    jmp loop        ;jmp back to loop

1:                  ;is palindrome
    ldi r24, 1
    clr r25

end:   
    pop r16
    pop r15
    pop r14               
    ret



;;; String length code from exericse 5

    .global Strlen

Strlen:
    ;;; uint16_t Strlen(char *arg)
    ;;; length stored in r24: r25
    mov r27, r25
    mov r26, r24
    clr r24
    clr r25

2:
    ld r0, X
    tst r0
    breq 1f
    adiw r26, 1
    adiw r24, 1
    jmp 2b

1:
    ret
