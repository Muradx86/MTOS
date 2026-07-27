global _fast
global _slow
_fast: ;Cpy esi to edi as much as ecx
	push edi
	push esi
	push ecx
	cld
	mov edi,0x3000
	mov esi,0
	mov ecx,0xffff
	rep movsd
	pop ecx
	pop esi
	pop edi
	ret
_slow: ;Do mem->reg and reg->mem
	push edi
	push esi
	push ecx
	mov edi,0x3000
	mov esi,0
_loop:
	mov dword [edi],esi
	inc esi
	inc edi
	dec ecx
	cmp ecx,0
	jl _loop
_end:
	pop ecx
	pop esi
	pop edi
	ret
