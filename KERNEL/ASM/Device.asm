;Device specific
global KbdHasKey
global remap_pic

KbdHasKey:
WLOOP:
	 IN AL,0x60
    AND AL,1
    	JZ WLOOP
  MOV EAX,1
    	RET

remap_pic:
        ;ICW 1
        MOV DX,0x20
        MOV AL,0x11
        OUT DX,AL

        MOV DX,0xA0
        MOV AL,0x11
        OUT DX,AL

        ;ICW2
        MOV DX,0x21
        MOV AL,0x20
        OUT DX,AL

        MOV DX,0xA1
        MOV AL,0x20
        OUT DX,AL

        ;ICW3 MASTER
        MOV DX,0x21
        MOV AL,4
        OUT DX,AL

        ;ICW3 SLAVE
        MOV DX,0xA1
        MOV AL,2
        OUT DX,AL

        ;ICW4
        MOV DX,0x21