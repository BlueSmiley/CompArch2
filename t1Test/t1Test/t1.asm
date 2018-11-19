.686                                ; create 32 bit code
.model flat, C                      ; 32 bit memory model
 option casemap:none                ; case sensitive

.data								;start of a data section
public g							;export variable g
g	DWORD	4						;declare g initialised to 4
.code

public		min		;make sure function  name is exported

min:	push    ebp             ; push frame pointer
        mov     ebp, esp        ; update ebp

		mov		eax, [ebp+8]	; min = p0
		cmp		eax, [ebp+12]	; if p1<min
		jle		min0			; 
		mov		eax, [ebp+12]	; min = p1
min0:   cmp		eax, [ebp+16]	; if p2 < min
		jle		min1
		mov		eax, [ebp+16]	; min = p2
min1:	mov     esp, ebp        ; restore esp
		pop     ebp             ; restore ebp
        ret     0               ; return

public p

p:		push    ebp             ; push frame pointer
        mov     ebp, esp        ; update ebp
		push	[ebp + 8]		;push p0
		push	[ebp + 12]		;push p1
		push	[ebp + 16]		;push p2
		call	min				;find min of p0,p1,p2
		add		esp, 12			;pop off parameters

		push	eax				;push in the result to params
		push	[ebp + 20]		;push param in frame p3
		push	[g]				;push g
		call	min				;find final min
		add		esp, 12			;pop off parameters

		mov     esp, ebp        ; restore esp
		pop     ebp             ; restore ebp
        ret     0               ; return

public		gcd

gcd:	push    ebp             ; push frame pointer
        mov     ebp, esp        ; update ebp

		mov		eax, [ebp + 12]	;get int b (dividend)
		mov		ecx, [ebp + 8] ;get int a	(divisor)
		test	ecx, ecx		;end if a == 0
		je		gcd1
		push	ecx				; a = dividend for inner function call
		xor     edx,edx			; Zero edx since div by 32 bit register causes dividend to be edx:eax. Unnescary here but w/e
		cdq						;sign extend across edx:eax for unsigned division apparently
		idiv	ecx				; edx:eax/ebx = a/b in eax and a%b remainder in edx
		push	edx				; a%b = p1 for function call. Remainder = divisor
		call	gcd
		add		esp,	8		; pop off parameters

gcd1:	mov     esp, ebp        ; restore esp
		pop     ebp             ; restore ebp
        ret     0               ; return

end