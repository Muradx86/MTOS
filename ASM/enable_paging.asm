section .text
global enable_paging
extern page_directory

enable_paging:
	XOR EAX,EAX
	MOV EAX,CR4
	OR EAX,16
	MOV CR4,EAX

	XOR EAX,EAX
	
	MOV EAX,CR0
	OR EAX,0x80000001
	MOV CR0,EAX
	RET
