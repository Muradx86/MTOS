#include "vga.h"
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "util.h"
#include "port.h"
#define WIDTH 80
#define HEIGHT 25

static volatile uint8_t line = 0;
static volatile uint8_t column = 0;
extern void colorize();
void init_cursor(uint8_t x,uint8_t y){
	outb(0x3D4,0xA);
	//outb(0x3D4,~(1<<5));
	outb(0x3D5,x);
	outb(0x3D4,0xB);
	outb(0x3D5,y);
}
void update_cursor(uint16_t x,uint16_t y){
	uint16_t addres = y*WIDTH + x;
	outb(0x3D4,0xF);
	outb(0x3D5,(uint8_t)(addres & 0xFF));
	outb(0x3D4,0xE);
	outb(0x3D5,(uint8_t)(addres >> 8));
}
void newline(void){
	if(line <= HEIGHT - 1){
		line++;
		column = 0;
		update_cursor(column,line);
	}
	else if(line == HEIGHT){
		colorize();
		line = 0; 
		column = 0;
		update_cursor(column,line);
	}
}
volatile uint16_t* vga_putchar(char c){
	static volatile uint16_t* videomem = (volatile uint16_t*)0xb8280;
	static volatile uint32_t vmem_dummy = 0xb8280;

	switch (c){
		case '\n':
			newline();
			break;
		case '\b':
			videomem[((1 << 4) + (1 << 6)) * line + column] = ' ' | (COLOR << 8);
			column--;
			if(column == 0){
				 line--;
				 column = 80;
			}
			if(line < 0)
				line = -line;
			update_cursor(column-1,line+4);
			break;
		default:
			videomem[((1 << 4) + (1 << 6)) * line + column++] = c | (COLOR << 8);

			if(column % WIDTH == 0){
				 line++;
				 column = 0;
			}
			update_cursor(column-1,line+4);
			break;
	}

	if(line == 24){
		colorize();
		videomem = (uint16_t*)0xb8000;
	}
	update_cursor(column-1,line+4);
	return videomem;
}
void printa(char * c){
	static volatile uint16_t* vga_mem = (volatile uint16_t*)0xb8000;
	while(*c){
		switch (*c){
			case '\n':
				newline();
				break;
			default:
				vga_mem[((1 << 6) + (1 << 4))* line + column ++] = *c | ((COLOR | 0x80) << 8);
				update_cursor(column,line);
				break;
		}
		c++;
	}
}
void puta(char c){
	volatile uint16_t* vga = (volatile uint16_t*)0xb8000;
	*vga++ = c | (0x30 << 8);
}
void putch(char c){
	static volatile uint16_t* vga_mem = (volatile uint16_t*)0xb8000;
	vga_mem[((1 << 6) + (1 << 4)) * line + column++] = c | ((COLOR | 0x80) << 8);
}
void printk(char* fmt,...){
	uint32_t val32;
	char* string;
	char array;
	char* p;
	va_list argp;
	va_start(argp,fmt);
	for(p = fmt;*p;p++){
		if(*p != '%'){
			putch(*p);
			continue;
		}
		switch (*++p){
			case 'U':
			case 'u':
				val32 = va_arg(argp,uint32_t);
				printa(utoa(val32,16));
				break;
			case '\n':
				newline();
				break;
			case 'S':
			case 's':
				string = va_arg(argp,char*);
				printa(string);
				break;
			/*case 'C':
			case 'c':
				array = va_arg(argp,char);
				printa(array);
				break;
			*/
			default:
				putch(*p);
				break;
		}
	}
	va_end(argp);
}
void coordinate_print(const char* s,uint32_t x,uint32_t y){
	volatile uint16_t* videomem = (volatile uint16_t*)0xb8000;
	uint8_t color = 0x8f;
	if(*s == ' ') color = 0x30;
	while(*s){
		videomem[((1 << 6) + (1 << 4)) * y + x++] = *s++ | (color << 8);
	}
	x = 0;
}
void drawSmiley(void){
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
void print_clock(uint64_t seconds){
	static volatile uint16_t* clck_vga = (volatile uint16_t*)0xB8050;
	*clck_vga = *(utoa(seconds,10)) | (COLOR << 8);
}
