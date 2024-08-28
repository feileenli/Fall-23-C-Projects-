;;; Feileen Li
;;; 118924120
;;; fli1234

;;; Implementation that computes the square root of an integer using bitwise algorithm

	.global isqrt
isqrt:

	cpi r24, 2	; check if n < 2
	brlo end 	; branch to end if n < 2

execute:
	push r24	; save n value 
	push r25

	lsr r24		; n >> 2
	lsr r24
	
	call isqrt	; isqrt(n>>2)
	lsl r24		; isqrt(n >> 2) << 1

	movw r30, r24	; small var
	inc r30		; large = small + 1
	mul r30, r30	; large squared
	movw r18, r0	;store large squared in r18
	clr r1

	pop r25		;popping prev saved n value
	pop r24

	cp r24, r18	; compare large squared to n
	brsh large	; if n is greater than or equal to large
	dec r30		; large - 1 = small
	movw r24, r30
	jmp end

large:
	movw r24, r30	; ret large
end:
	ret 
