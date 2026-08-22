#include <stdint.h>
#include <stdbool.h>
#include "vga.h"
#include "port.h"
#include "keyboard.h"
#include "util.h"
#include "ExternASM.h"
#include "MAllocFree.h"
#define PS2_SC 0x60
#define PS2_R 0x64
#define BREAK_CODE_NUM 0x80
#define LSHIFT 0x2A
#define RSHIFT 0x36
#define LCTRL 0x1D
#define BKSPC 0xE
#define ENTER 0x1C
volatile char queu[2]={0};
static volatile int queu_cnt;
static const char lookup_norml[] = {
	1,27,'1','2','3','4','5','6','7','8','9','0','-','=',
	'\b',
	'\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
	0, 'a','s','d','f','g','h','j','k','l',
	';',0x28,0,
	0,43,'z','x','c','v','b','n','m',',','.',0,//53
	0,0,0,0/*57*/,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0x48,0,0,0x4B,0,0x4D,0,0,0x50,0
};
static const char lookup_shift[] = {
	1,27,'!','@','#','$','%','^','&','*','(',')','_','+',
	0,
	0,'Q',     
	'W','E','R','T','Y','U',
	'I','O','P','{','}','\n',
	0,'A','S','D',
	'F','G','H','J','K','L',
	':','"',0,
	0,43,'Z','X','C','V','B','N','M','<','>','|',
	 '?',0,0,32,0,0,0xE3,0xE9,0,0,0,0,0,
	0,0,0,0x48,0,0,0x4B,0,0x4D,0,0,0x50,0
};
bool kbd_has_key()
{
	if(inb(0x64)&2)
		return TRUE;
	else
		return FALSE;
}
static inline void corner_print(uint8_t scancode,const char* tbl)
{
	*(volatile uint8_t*)0xB809E = tbl[scancode];
	*(volatile uint8_t*)0xB809F = COLOR;
}
void ins_queu(char scancode)
{
	queu[0]=scancode;
}
char get_char()
{
	//return queu[0];
	return (*(volatile char*)0xB809E);
}
bool shift_clicked = false;
bool ctrl_clicked = false;
bool backspace = false;
bool caps_lock = false;
void keyboard_handler()
{	
	volatile uint8_t scancode_tbl = inb(PS2_SC);
	volatile uint8_t scancode_raw;
	
	bool clicked = !(scancode_tbl & BREAK_CODE_NUM);	

	switch (scancode_tbl){//Rshift 0x36 Lshift 0x2A
		case LSHIFT:
			shift_clicked = clicked; 
			break;
		case RSHIFT:
			shift_clicked = clicked; 
			break;
		case 0xAA:
			shift_clicked = false;
			break;
		case 0xB6:
			shift_clicked = false;
			break;
		case LCTRL:
			ctrl_clicked = clicked;
			break;
		case 0x9D:
			ctrl_clicked = false;
			 break;
		case 0xE:
			backspace = clicked; 
			break;
		case 0x8E:
			backspace = false; 
			break;
		case 0x3A:
			caps_lock = true;
			break;
		case 0xBA:
			caps_lock = false;
			break;
		default:
			break;
	}
 	if(clicked && shift_clicked && scancode_tbl != LSHIFT && scancode_tbl != RSHIFT){//Shift letters
 		scancode_raw = lookup_shift[scancode_tbl]; 		
		vga_putchar(scancode_raw);
		corner_print(scancode_tbl,lookup_shift);
 	}
 	else if(clicked && scancode_tbl != LSHIFT && scancode_tbl != LCTRL && scancode_tbl!=RSHIFT){//Normal letters
 		scancode_raw = lookup_norml[scancode_tbl];
		vga_putchar(scancode_raw);
		corner_print(scancode_tbl,lookup_norml);
		ins_queu(scancode_raw);
 	}
}
