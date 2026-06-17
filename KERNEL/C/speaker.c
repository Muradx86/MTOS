#include <stdint.h>
#include "port.h"
#include "pit.h"
#include "util.h"
extern void random_hw();
/* Notes macros */
#define DO 100
#define RE 98
#define MI 95
#define FA 90
#define SOL 110
#define LA 87
#define SI 80
#define LOW 76

void soundTest(uint32_t freq){
	outb(0x43,0xb6);
	
	uint8_t readset_status = inb(0x61);
	readset_status |= 3;
	outb(0x61,readset_status);
	
	uint32_t divider = FREQUENCY / freq;
	
	outb(0x42,(uint8_t)(divider) & 0xFF);
	outb(0x42,(uint8_t)(divider >> 8));
	//if is not equal to inital value, set again(H\L)
	//uint8_t CheckStatus = inb(0x61);

	/*if(CheckStatus != (inb(0x61) | 3)){
		outb(0x61,(CheckStatus) | 3);
	}
	*/
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
void readHymn(void)
{
	uint8_t random;
	for(uint32_t i=0;i<0xFFFFFF;i+=2){
		random=(randomU8());
		if(random%2==0)
			random=randomU16();
		if(random==0)
			continue;
		soundTest(random);
	}
}
