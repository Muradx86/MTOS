section .text
global testif
global random_hw

global rdrandU8
global rdrandU16
global rdrandU32
testif:
	push ebp
	mov ebp,esp
	xor edx,edx
	mov eax,1
	cpuid
	pop ebp
	ret
random_hw:
	mov edi,0x8000
	rdrand ax
	and ax,0xff
	mov word [edi],ax
	ret
rdrandU8:
	push edi
	push eax
	mov edi,0x8000
	rdrand ax
	and ax,0xff
	mov byte [edi],al
	pop eax
	pop edi
	ret
rdrandU16:
	push edi
	push eax
	mov edi,0x8000
	rdrand ax
	mov word [edi],ax
	pop eax
	pop edi
rdrandU32:
	push edi
	push eax
	mov edi,0x8000
	rdrand eax
	mov dword [edi],eax
	pop eax
	pop edi
	ret
