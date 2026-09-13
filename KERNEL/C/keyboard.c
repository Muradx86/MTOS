#include <stdint.h>
#include <stdbool.h>
#include "vga.h"
#include "port.h"
#include "keyboard.h"
#include "util.h"
#include "ExternASM.h"
#include "MAllocFree.h"
#include "pit.h"

#define PS2_SC 0x60
#define PS2_REG 0x64
#define BREAK_CODE_NUM 0x80
#define LSHIFT 0x2A
#define RSHIFT 0x36
#define LCTRL 0x1D
#define BKSPC 0xE
#define ENTER 0x1C

volatile int i=0;
volatile char queu[]={0};

static const char lookup_norml[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0
};

static const char lookup_shift[] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0
};

void kbd_sendcmd(uint8_t port,uint8_t cmd_byte)
{
	if(!(inb(PS2_REG)&2)){
		outb(port,cmd_byte);
		return;
	}
}

void keyboard_init(void)
{
	kbd_sendcmd(PS2_SC,0xF0);
	kbd_sendcmd(PS2_REG,0x1); //Set table 1
	kbd_sendcmd(PS2_SC,0xED); //Turn on LEDs
}

static inline void corner_print(uint8_t scancode,const char* tbl)
{
	*(volatile uint8_t*)0xB809E = tbl[scancode];
	*(volatile uint8_t*)0xB809F = COLOR;
}

void push_queu(uint8_t c)
{
	queu[i]=c;
	i++;
}

char get_char()
{
	uint8_t raw_sc;
	if(KbdHasKey()){
		raw_sc=inb(PS2_SC);
		if(lookup_norml[raw_sc]&&raw_sc<128)
			return lookup_norml[raw_sc];
	}
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
 	}
}
