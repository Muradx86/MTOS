;Device specific
global KbdHasKey
KbdHasKey:
WLOOP:
	IN AL,0x64
	AND AL,1
	JZ WLOOP
	MOV EAX,1
	RET
