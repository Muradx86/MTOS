global rdrandU8
global rdrandU16
global rdrandU32
global random_hw
global testif
global KMemcpy
global KMemset
global ComplementBit
global colorize
global colorize_dbg
global Sin
global Cos
global Round
global Sqrt
global ToU32
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
KMemset:
	push ebp
	mov ebp,esp
	push eax
	push ecx
	push edi
	mov dword edi,[ebp+8]
	mov ebx,edi
	mov byte al,[ebp+12]
	mov dword ecx,[ebp+16]
	rep stosb
	pop edi
	pop ecx
	pop eax
	pop ebp
	mov eax,ebx
	ret
KMemcpy:
	push ebp
	mov ebp,esp
	push edi
	push esi
	push ecx
	mov dword esi,[ebp+8]
	mov dword edi,[ebp+12]
	mov eax,edi
	mov dword ecx,[ebp+16]
	rep movsb
	pop ecx
	pop esi
	pop edi
	pop ebp
	ret
ComplementBit:
	push ebp
	mov ebp,esp
	mov byte al,[ebp+12]
	movzx eax,al
	btc dword [ebp+8],eax
	pop ebp
	ret
colorize:
	mov edi,0xb8000
	mov ax,' ' | (0x7E << 8)
	mov ecx,2000
	cld
	rep stosw
	ret

colorize_dbg:
	mov edi,0xb8000
	mov ax,0
	mov ecx,4000
	cld
	rep stosb
	ret
Sin:
	push ebp
	mov ebp,esp
	fild dword [ebp+8]
	fsin
	frndint
	fistp dword [ebp-12]
	mov dword eax,[ebp-12]
	pop ebp
	ret
Cos:
	push ebp
	mov ebp,esp
	fild dword [ebp+8]
	fcos
	frndint
	fistp dword [ebp-12]
	mov dword eax,[ebp-12]
	pop ebp
	ret
Round:
	push ebp
	mov ebp,esp
	fild dword [ebp+8]
	frndint 
	fistp dword [ebp-12]
	mov dword eax,[ebp-12]
	pop ebp
	ret
Sqrt:
	push ebp
	mov ebp,esp
	fild dword [ebp+8]
	fsqrt
	fistp dword [ebp-12]
	mov dword eax,[ebp-12]
	pop ebp
	ret
ToU32:
	push ebp
	mov ebp,esp
	mov word ax,[ebp+8]
	movzx eax,ax
	pop ebp
	ret
