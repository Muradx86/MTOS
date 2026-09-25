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
    1, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    1, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    1, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 1,
    '*', 1, ' ', 1
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
bool shift_clicked=false;
char getchar()
{
	while(!KbdHasKey());
	char raw_sc=inb(PS2_SC);
	if(raw_sc==RSHIFT||raw_sc==LSHIFT)
		shift_clicked=true;
	if(raw_sc<127&&!(raw_sc&0x80)){
		if(shift_clicked){
			shift_clicked=false;
			return lookup_shift[(int)raw_sc];
		}
		return lookup_norml[(int)raw_sc];
	}
	else 
		return 0;
}

char get_char()
{
	uint8_t raw_sc;
	bool clicked;
	if(KbdHasKey()){
		raw_sc=inb(PS2_SC);
		clicked=!(raw_sc&0x80);
		if(lookup_norml[raw_sc]!=0&&raw_sc<0x80&&clicked)
			return lookup_norml[raw_sc];
	}
}
