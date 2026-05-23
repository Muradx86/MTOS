#include <stdint.h>
#include "port.h"
void outb(uint16_t port,uint8_t val){
	asm volatile("outb %b0,%w1"
		      :: "a"(val),"Nd"(port)
		      :"memory");
}

void outw(uint16_t port,uint16_t val){
	asm volatile("outw %w0,%w1"
		      :: "a"(val),"Nd"(port)
		      :"memory");
}

uint8_t inb(uint16_t port){
	uint8_t val;
	asm volatile("inb %w1,%b0" 
		      :"=a"(val)
		      :"Nd"(port)
		      :"memory");
	return val;
}

uint16_t inw(uint16_t port){
	uint16_t val;
	asm volatile("inw %w1,%w0" 
		      :"=a"(val)
		      :"Nd"(port)
		      :"memory");
	return val;
}

void io_wait(void){
	outb(0x80,0);
}
