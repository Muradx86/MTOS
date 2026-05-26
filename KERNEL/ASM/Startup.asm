BITS 16
extern kernel
global start
start:
	CLI
	LGDT [BHDD_DESCRIPT_GDT]
	XOR EAX,EAX
	MOV EAX,CR0
	BTC EAX,0
	MOV CR0,EAX
	JMP 0x10:SEGRELOAD
BITS 32
SEGRELOAD:
	MOV AX,0x18
	MOV DS,AX
	MOV ES,AX
	MOV SS,AX
	MOV ESP,0x7FEFC
	MOV FS,AX
	MOV GS,AX
	JMP kernel
	HLT
	JMP $
ALIGN 8
BHDD_GDT_START:
	DQ 0

	DW 0xFFFF			;limit quad
	DW 0x0000			;base quad
	DB 0x00				;base byte
	DB 0x9A				;type byte
	DB 0xCF				;flags
	DB 0x00				;base byte
	
	DW 0xFFFF			;limit quad
	DW 0x0000			;base quad
	DB 0x00				;base byte
	DB 0x92				;type byte		
	DB 0xCF				;flags
	DB 0x00
BHDD_GDT_END:

BHDD_DESCRIPT_GDT:
	DW BHDD_GDT_END - BHDD_GDT_START - 1
	DD BHDD_GDT_START
