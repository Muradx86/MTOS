section .text
global colorize
global colorize_dbg
colorize:
	push ebp
	mov ebp,esp	
	mov edi,0xb8000
	mov ax,0x33
	mov ecx,4000
	cld
	rep stosb
	XOR EDX,EDX
	pop ebp
	ret

colorize_dbg:
	mov edi,0xb8000
	mov ax,0
	mov ecx,4000
	cld
	rep stosb
	ret
