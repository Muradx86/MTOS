section .text
global DieScreen
extern drawSmiley

DieScreen:
	push ebp
	mov ebp,esp

	mov ecx,4000
	mov edi,0xb8000
	mov ax,0x01020
	cld				;esi++
	rep stosw

	call drawSmiley
	xor edi,edi
	
	pop ebp
	ret
