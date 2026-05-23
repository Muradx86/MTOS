#include <stdint.h>
#include "port.h"
#include "pit.h"

/* Notes macros */
#define DO 220
#define RE 280
#define MI 290
#define FA 270
#define SOL 275
#define LA 245
#define SI 253
#define LOW 213

void soundTest(uint32_t divide_to_this){
	outb(0x43,0xb6);
	
	uint8_t readset_status = inb(0x61);
	readset_status |= 3;
	outb(0x61,readset_status);
	
	uint32_t divider = FREQUENCY / divide_to_this;
	
	outb(0x42,(uint8_t)(divider) & 0xFF);
	outb(0x42,(uint8_t)(divider >> 8));
	//if is not equal to inital value, set again(H\L)
	uint8_t status_checker_initial = inb(0x61);

	if(status_checker_initial != (inb(0x61) | 3)){
		outb(0x61,(status_checker_initial) | 3);
	}
}

void Do(void){
	uint32_t freq = FREQUENCY / 0x1194;
	outb(0x43,0xb6);
	outb(0x42,(uint8_t)(freq) & 0xFF);
	outb(0x42,freq >> 8);
}

void Re(void){
	uint32_t freq = FREQUENCY / 0xFAD;
	outb(0x43,0xb6);
	outb(0x42,(uint8_t)(freq) & 0xFF);
	outb(0x42,freq >> 8);
}

void stop_sound(void){
	uint8_t get = inb(0x61);

	get &= 0xfc;			//do zero lower bits
	outb(0x61,get);
}

void wait(void){
	static volatile uint32_t timeout = 1000000;
	while(timeout) timeout--;
}

void readHymn(void){
	
}
