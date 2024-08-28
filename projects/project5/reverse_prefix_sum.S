;;; Feileen Li
;;; 118924120
;;; fli1234

.data
pctd:
	.asciz "%d"		;format stirng

s_pctd:
	.asciz " %d"		;format string
	
	.text

;;;transform an array by adding value at an index to sum of remainder of array

.global reverse_prefix_sum

reverse_prefix_sum:

	movw r26, r24			;initialize x pointer 
	ld r18, x			;load r18 with *a
	cpi r18, -1			;check if *a = -1
	breq finished			;branch to finished if it is

	push r26			;save x pointer to a
	push r27
	push r18 			;save *a
	adiw r24, 1			;increment pointer
	call reverse_prefix_sum		;recursive call
	pop r18				;restore *a
	pop r27
	pop r26				;restore X 

	add r24, r18			;add sum
	adc r25, r1

	st X, r24			;store sum
	
	jmp done

finished:
	ldi r24, 0
	ldi r25, 0
	
done:
	ret

;;;print array out

.global print_array

print_array:
	
	movw r26, r24			;copy a to X pointer
	cpi r24, -1			;compare *a to -1
	breq end			;branch to end if value is -1

	push r26			;save *a 
	push r27
	ld r24, X			;load a value to r24
	clr r25
	call pint			;print first value of array
	pop r27				;restore *a
	pop r26

loop:
	adiw r26, 1			;increment pointer
	ld r24, X			;load r24 with post increment
	clr r25

	cpi r24, -1			;compare *a to -1
	breq end			;branch to end if value is -1

	push r26			;save *a
	push r27 
	call pint_s			;print other values of array
	pop r27  			;restore *a
	pop r26

	jmp loop			;loop back

end:
		ldi r24, 10		;printing new line
		clr r25
		call putchar

		ret

;;; prints an integer value, r22/r23 have the format string
pint:

    ldi r22, lo8(pctd)      ; Lower byte of the string address
    ldi r23, hi8(pctd)      ; Higher byte of the string address
    push r25               ; Save r25 (required for printf call)
    push r24               ; Save r24 (required for printf call)
    push r23               ; Save r23 (required for printf call)
    push r22               ; Save r22 (required for printf call)
    call printf            ; Call printf to print the integer
    pop r22                ; Restore r22
    pop r23                ; Restore r23
    pop r24                ; Restore r24
    pop r25                ; Restore r25
    ret                    ; Return from the subroutine

;;; prints an integer value followed by a space
pint_s:

    ldi r22, lo8(s_pctd) ; Lower byte of the string address
    ldi r23, hi8(s_pctd) ; Higher byte of the string address
    push r25               ; Save r25 (required for printf call)
    push r24               ; Save r24 (required for printf call)
    push r23               ; Save r23 (required for printf call)
    push r22               ; Save r22 (required for printf call)
    call printf            ; Call printf to print the integer followed by a space
    pop r22                ; Restore r22
    pop r23                ; Restore r23
    pop r24                ; Restore r24
    pop r25                ; Restore r25
    ret                    ; Return from the subroutine
