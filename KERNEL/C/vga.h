#ifndef VGA_H
#define VGA_H

#define COLOR 0x30

#include <stdint.h>
void print(char* c);
void coordinate_print(const char* s,uint32_t x,uint32_t y);
void drawSmiley(void);
void printk(char* fmt,...);

void printF(char* fmt,...);
volatile uint16_t* vga_putchar(char c);
void update_cursor(uint16_t x,uint16_t y);
void init_cursor(uint8_t x,uint8_t y);
void printa(char * c);
void putch(char c);
void print_clock(uint64_t seconds);
void print_shell(char* c);

#endif
