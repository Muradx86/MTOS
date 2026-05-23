section .text
global testif

testif:
	push ebp
	mov ebp,esp
	xor edx,edx
	mov eax,1
	cpuid
	pop ebp
	ret
