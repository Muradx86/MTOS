#include "vga.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "util.h"
#include "port.h"
#include "ExternASM.h"
#define BUF_BACK 0x4000
#define WIDTH 80
#define HEIGHT 25
struct Cursor{
	int32_t line,column;
};
struct Cursor* Cursor;
void newline()
{
	if(Cursor->line <= HEIGHT - 3){
		Cursor->line++;
		Cursor->column = 0;
		update_cursor(Cursor->column,Cursor->line);
		return;
	}
}
void init_cursor(uint8_t x,uint8_t y)
{
	outb(0x3D4,0xA);
	outb(0x3D5,x);
	outb(0x3D4,0xB);
	outb(0x3D5,y);
}
void update_cursor(uint16_t x,uint16_t y)
{
	uint16_t addres = y*WIDTH + x;
	outb(0x3D4,0xF);
	outb(0x3D5,(uint8_t)(addres & 0xFF));
	outb(0x3D4,0xE);
	outb(0x3D5,(uint8_t)(addres >> 8));
}

void putch(char c)
{
	static volatile uint16_t* vga_mem = (volatile uint16_t*)0xb8000;
	if(c=='\n')
		newline();
	vga_mem[WIDTH * (Cursor->line) + (Cursor->column)++] = c | (COLOR << 8);
}
void vga_putchar(char c)
{//Mostly used for keyboard.
	static volatile uint16_t* videomem = (volatile uint16_t*)0xb8000;
	switch (c){
		case '\n':
			newline();
			break;
		case '\b':
			videomem[WIDTH * (Cursor->line) + (Cursor->column)] = ' ' | (COLOR << 8);
			Cursor->column--;
			if(Cursor->column == 0){
				 (Cursor->line)--;
				 Cursor->column = 80;
			}
			if(Cursor->line < 0)
				Cursor->line = -Cursor->line;
			update_cursor(Cursor->column,Cursor->line);
			break;
		case '\t':
			Cursor->column += 4;
			break;
		case 0x4B:
			Cursor->column--;
			if(Cursor->column == 0){
				Cursor->line--;
				Cursor->column = 79;
			}
			break;
		case 0x4D:
			Cursor->column++;
			if(Cursor->column == 79){
				Cursor->line++;
				Cursor->column = 0;
			}
			break;
		case 0x48:
			Cursor->line--;
			break;
		case 0x50:
			Cursor->line++;
			break;
		case 0x39:
			Cursor->column++;
			break;
		default:
			putch(c);
			if(Cursor->column == WIDTH){
				 (Cursor->line)++;
				 Cursor->column = 0;
			}
			break;
	}
	update_cursor(Cursor->column,Cursor->line);
}
void print(char * c)
{	
	while(*c){
		switch (*c){
			case '\n':
				newline();
				break;
			default:
				putch(*c);
				update_cursor(Cursor->column,Cursor->line);
				break;
		}
		c++;
	}
}
void coordinate_print(const char* s,uint32_t x,uint32_t y)
{
	volatile uint16_t* videomem = (volatile uint16_t*)0xb8000;
	while(*s){
		videomem[WIDTH * y + x++] = *s++ | (COLOR << 8);
	}
}
void printk(char* fmt,...)
{
	uint32_t val32,intgr,ptr_int;
	char* p,*string,**ptr_temp;
	va_list argp;
	va_start(argp,fmt);
	for(p = fmt;*p;p++){
		if(*p=='\n'){
			vga_putchar('\n');
			continue;
		}
		if(*p != '%'){
			putch(*p);
			continue;
		}
		switch (*++p){
			case 'U':
			case 'u':
				val32 = va_arg(argp,uint32_t);
				print(utoa(val32,16));
				break;
			case 'S':
			case 's':
				string = va_arg(argp,char*);
				print(string);
				break;
			case 'D':
			case 'd':
				intgr = va_arg(argp,uint32_t);
				print(utoa(intgr,10));
				break;
			case 'p':
			case 'P':
				ptr_temp=(char**)va_arg(argp,char*);
				ptr_int=offset(ptr_temp);
				print(utoa(ptr_int,16));				
				break;
			default:
				putch(*p);
				break;
		}
	}
	va_end(argp);
}
void drawSmiley()
{
	volatile unsigned char* videomem = (volatile unsigned char*)0xb8140;
	volatile uint8_t eyes_times = 0;
	for(volatile uint32_t index = 0;index < 2000;index++){
		if((index % 0x140) == 0){
			videomem[index] = '0';
			videomem[index + 1] = 0x0;
			eyes_times++;
			if(eyes_times == 2){
				goto mouth;
			}
		}	
	}
	mouth:
		//piece 1
		*(volatile uint8_t*)0xb814a = '0';
		*(volatile uint8_t*)0xb814b = 0x0;

		//piece 2
		*(volatile uint8_t*)0xb81e8 = '0';
		*(volatile uint8_t*)0xb81e9 = 0x0;

		//piece 3
		*(volatile uint8_t*)0xb828a = '0';
		*(volatile uint8_t*)0xb828b = 0x0;
}
