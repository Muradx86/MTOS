#include <stdint.h>

#define HEIGHT 25
#define WIDTH 80
#define COLOR_DEF 0x30
static uint8_t line = 0;
static uint8_t column = 0;

void newline(void){
	line++;
	column = 0;	
}

void print(char * c){
	static volatile uint16_t* vga_mem = (volatile uint16_t*)0xb8000;
	while(*c){
		switch (*c){
			case '\n':
				newline();
				break;
			default:
				vga_mem[WIDTH * line + column] = *c | (COLOR_DEF << 8);
				column++;
				break;
		}
	}
}
