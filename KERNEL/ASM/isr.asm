global isr0
global isr4
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr16
global isr18
global isr19
global isr38

global irq32
global irq33
global irq45

extern get_char
extern keyboard_handler
extern mouse_handler
extern IRQ32Handler
extern InterruptHandler
extern readHymn
extern MOV_SPRITE
extern debug_init
;=================SOFTWARE INTERRUPTS======================
isr0:
	POP EAX
	JMP ISR_CALL_NEC
isr4:
	POP EAX
	JMP ISR_CALL_NEC
isr7:
	POP EAX
	JMP ISR_CALL_NEC
isr8:
	;ADD ESP,4
	;POP EAX
	;JMP ISR_CALL_EC
	JMP DBG
isr6:
	ADD ESP,4
	POP EAX
	JMP ISR_CALL_EC
isr11:
	ADD ESP,4
	POP EAX
	JMP ISR_CALL_EC
isr12:
	ADD ESP,4
	POP EAX
	JMP ISR_CALL_EC
isr13:
	ADD ESP,4
	POP EAX
	JMP ISR_CALL_EC
isr14:
	ADD ESP,4
	POP EAX
	JMP ISR_CALL_EC

isr16:
	POP EAX
	JMP ISR_CALL_NEC

isr18:
	ADD ESP,4
	POP EAX
	JMP ISR_CALL_EC

isr19:
	POP EAX
	JMP ISR_CALL_NEC

isr10:
	ADD ESP,4
	POP EAX
	JMP ISR_CALL_EC

;===================HARDWARE INTERRUPTS==================
irq32:
	CALL IRQ32Handler
	MOV AL,0x20
	MOV DX,0x20
	OUT DX,AL
	IRETD
	
irq33:
	CLI
;	CALL keyboard_handler
	MOV EBX,0xB8000
	MOV WORD [EBX],'I'|(0x9f<<8)
	MOV AL,0x20
	MOV DX,0x20
	OUT DX,AL
	STI
	IRETD
	
irq45:
	CLI
	CALL mouse_handler
	MOV AL,0x20
	MOV DX,0x20
	OUT DX,AL
	STI
	IRETD

DBG:
	;MOV DWORD [ESP+4],debug_init
	CALL debug_init
	IRETD

ISR_CALL_NEC:
	CLI
	MOV EBX,0x9000
	PUSH EBX
	MOV [EBX],EAX
	CALL InterruptHandler
	POP EBX
	HLT
	IRETD
ISR_CALL_EC:
	CLI
	MOV EBX,0x9000
	MOV DWORD [EBX],EAX
	CALL InterruptHandler
	HLT
	ADD ESP,4
	IRETD

idle:
	RET
