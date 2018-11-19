option casemap:none                ; case sensitive
includelib	legacy_stdio_definitions.lib
extrn	printf:near
.data								;start of a data section
public g							;export variable g
g	QWORD	4						;declare g initialised to 4
fxp2	db	'a = %I64d b = %I64d c = %I64d d = %I64d e = %I64d sum = %I64d',0AH, 00H	;Ascii format string
broke	db	'broken',0AH, 00H	;Ascii format string
.code

public		min		;make sure function  name is exported

min:	mov		rax, rcx	; min = p0
		cmp		rax, rdx	; if p1<min
		jle		min0		; 
		mov		rax, rdx	; min = p1
min0:   cmp		rax, r8		; if p2 < min
		jle		min1
		mov		rax, r8	; min = p2
min1:   ret     0               ; return

public		p

p:		sub		rsp, 32			;allocate shadow space
		call	min				;find min of p0,p1,p2

		mov		rcx, rax		;push in the result to params
		mov		rdx, r9			; push in last param
		mov		r8,	[g]			;push g
		call	min				;find final min

		add		rsp, 32			;deallocate shadow space
        ret     0               ; return

public		gcd

gcd:	mov		r8,	rdx			;move divisor to r8
		mov		rax, rcx		;move dividend to rax
		test	r8, r8	;end if divisor == 0
		je		gcd1
		cqo						;sign extend across rdx:rax for unsigned division apparently
		idiv	r8				; edx:eax/ebx = a/b in rax and a%b remainder in rdx
		mov		rcx, r8			; p0 = old divisor and new dividend
		call	gcd

gcd1:	ret     0               ; return

public		q

q:		sub		rsp, 56			;allocate shadow space
		mov		rax, rcx		;sum = a		
		add		rax, rdx		; add a+b
		add		rax, r8			; a+b+c
		add		rax, r9			; a+b+c+d
		add		rax, [rsp+96]	; a+b+c+d+e

		mov		[rsp+88],	rax		;save sum in shadow space
		mov		[rsp+48],	rax		;push param 7
		mov		rax,	[rsp+96]	;for pushing
		mov		[rsp+40],	rax		;push param 6
		mov		[rsp+32],	r9				;push param 5
		mov		r9, r8			;param 4 in r9
		mov		r8, rdx			;param 3 in r8
		mov		rdx, rcx		;param 2 in rdx
		lea		rcx, fxp2		;printf string
		call	printf
		mov		rax, [rsp+88]	; get back sum
		add		rsp, 56			;deaallocate all space
		ret		0

public		qns

qns:	sub		rsp, 32			;allocate shadow space
		lea		rcx, broke		;printf string
		call	printf
		add		rsp, 32			;deaallocate all space
		xor		rax, rax
		ret		0
end