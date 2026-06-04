#include <stdint.h>
#include <stdbool.h>
#include "vga.h"
#include "port.h"
#include "keyboard.h"
#include "util.h"
#define PS2_SC 0x60
#define PS2_R 0x64
#define MAX 20
#define BREAK_CODE_NUM 0x80
#define CORNER8 0xb809e		
#define LSHIFT 0x2A
#define RSHIFT 0x36
#define LCTRL 0x1D
#define BKSPC 0xE
#define ENTER 0x1C
extern void PIANO_MAIN();
const char lookup_norml[] = {
	1,27,'1','2','3','4','5','6','7','8','9','0','-','=',
	'\b',
	0,'q',     
	'w','e','r','t','y','u',
	'i','o','p','[',']','\n',
	0,'a','s','d',
	'f','g','h','j','k','l',
	';','"',0,
	0,43,'z','x','c','v','b','n','m',',','.',92,
	0,0,0,32,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0x48,0,0,0x4B,0,0x4D,0,0,0x50,0
};
const char lookup_shift[] = {
	1,27,'!','@','#','$','%','^','&','*','(',')','_','+',
	0,
	0,'Q',     
	'W','E','R','T','Y','U',
	'I','O','P','{','}','|',
	0,'A','S','D',
	'F','G','H','J','K','L',
	':',0,0,
	0,43,'Z','X','C','V','B','N','M','<','>','|',
	'?',0,0,32,0,0,0xE3,0xE9,0,0,0,0,0,0,
	0,0,0,0x48,0,0,0x4B,0,0x4D,0,0,0x50,0,0
};
static inline void corner_print(uint8_t scancode,const char* tbl)
{
	*(volatile uint8_t*)CORNER8 = *(tbl + scancode);
	*(volatile uint8_t*)0xb809f = 0x8f;
}
volatile bool shift_clicked = false;
volatile bool ctrl_clicked = false;
volatile bool backspace = false; 

void keyboard_handler(void)
{	
	struct Kbd{
		void (*pCP)(uint8_t,const char*);
		volatile uint16_t* (*pPUTCHAR)(char);
	};
	struct Kbd* Kbd;
	Kbd->pCP = &corner_print;
	Kbd->pPUTCHAR = &vga_putchar;
	
	volatile uint8_t scancode_tbl = inb(PS2_SC);
	volatile uint8_t scancode_raw;
	
	bool clicked = !(scancode_tbl & BREAK_CODE_NUM);	

	switch (scancode_tbl){//Rshift 0x36 Lshift 0x2A
		case LSHIFT:
			shift_clicked = clicked; coordinate_print("SHIFT",75,0); break;
		case RSHIFT:
			shift_clicked = clicked; coordinate_print("SHIFT",75,0); break;
		case 0xAA:
			shift_clicked = false; kmemset((void*)0xb8096,10,0x33); break;
		case 0xB6:
			shift_clicked = false; kmemset((void*)0xb8096,10,0x33); break;
		case LCTRL:
			ctrl_clicked = clicked; coordinate_print("CTRL",74,0); break;
		case 0x9D:
			ctrl_clicked = false; kmemset((void*)0xb8096,10,0x33); break;
		case 0xE:
			backspace = clicked; coordinate_print("BCKSPC",74,0); break;
		case 0x8E:
			backspace = false; kmemset((void*)0xb8094,12,0x33); break;
		case ENTER:
			coordinate_print("ENTER",75,0); break;
		case 0x9C:
			kmemset((void*)0xb8096,10,0x33); break;
		case 0x58:
			PIANO_MAIN();
			break;
		default:
			break;
	}
 	if(clicked && shift_clicked && scancode_tbl != LSHIFT && scancode_tbl != RSHIFT){//Shift letters
 		scancode_raw = lookup_shift[scancode_tbl]; 		
		(*vga_putchar)(scancode_raw);
		(*corner_print)(scancode_tbl,lookup_shift);
 	}
 	else if(clicked && scancode_tbl != LSHIFT && scancode_tbl != LCTRL){//Normal letters
 		scancode_raw = lookup_norml[scancode_tbl];
		(*vga_putchar)(scancode_raw);
		(*corner_print)(scancode_tbl,lookup_shift);

 	}
 	else if(clicked && shift_clicked && ctrl_clicked && scancode_tbl != LSHIFT && scancode_tbl != RSHIFT && scancode_tbl != LCTRL){//scientific
 		scancode_raw = lookup_shift[scancode_tbl];		
		(*vga_putchar)(scancode_raw);
		(*corner_print)(scancode_tbl,lookup_shift);
 	}
}
