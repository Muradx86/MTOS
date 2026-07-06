#include <stdint.h>
#include <stdbool.h>
#include "vga.h"
#include "port.h"
#include "keyboard.h"
#include "util.h"
#include "ExternASM.h"
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
extern void readHymn();
struct Mode{
	uint8_t hymn;
};
struct Mode Mod;
const char lookup_norml[] = {
	1,27,'1','2','3','4','5','6','7','8','9','0','-','=',
	'\b',
	'\t','q',     
	'w','e','r','t','y','u',
	'i','o','p','[',']','\n',
	0, 'a','s','d',
	'f','g','h','j','k','l',
	';',0x27,0,
	0,43,'z','x','c','v','b','n','m',',','.',92,
	0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,
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
	':','"',0,
	0,43,'Z','X','C','V','B','N','M','<','>','|',
	 '?',0,0,32,0,0,0xE3,0xE9,0,0,0,0,0,
	0,0,0,0,0x48,0,0,0x4B,0,0x4D,0,0,0x50,0
};
void BufWrite(char* buf,uint32_t n,char scancode)
{
	char* addres = (char*)(buf + n);
	*addres = scancode; 
}
static inline void corner_print(uint8_t scancode,const char* tbl)
{
	*(volatile uint8_t*)CORNER8 = *(tbl + scancode);
	*(volatile uint8_t*)0xb809f = COLOR;
}
char getch(void)
{
	return lookup_norml[inb(PS2_SC)];
}
char* getln(char* s)
{
	static const uint32_t limit = 80;
	uint32_t i;
	char c;
	for(i=0;i<limit-1 &&(c=getch())!='\n';++i)
		s[i] = c;
	if(c=='\n')
		s[i] = '\0';
	return s;
}
bool shift_clicked = false;
bool ctrl_clicked = false;
bool backspace = false;

void keyboard_handler(void)
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
		default:
			break;
	}
 	if(clicked && shift_clicked && scancode_tbl != LSHIFT && scancode_tbl != RSHIFT){//Shift letters
 		scancode_raw = lookup_shift[scancode_tbl]; 		
		vga_putchar(scancode_raw);
		corner_print(scancode_tbl,lookup_shift);
 	}
 	else if(clicked && scancode_tbl != LSHIFT && scancode_tbl != LCTRL){//Normal letters
 		scancode_raw = lookup_norml[scancode_tbl];
		vga_putchar(scancode_raw);
		corner_print(scancode_tbl,lookup_norml);
 	}
}
