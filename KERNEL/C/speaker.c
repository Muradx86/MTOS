#include <stdint.h>
#include "port.h"
#include "pit.h"
#include "util.h"
typedef float F64;
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

void soundTest(float freq){
	outb(0x43,0xb6);
	
	uint8_t readset_status = inb(0x61);
	readset_status |= 3;
	outb(0x61,readset_status);
	
	float divider = FREQUENCY / freq;
	
	outb(0x42,(uint8_t)divider & 0xFF);
	outb(0x42,divider * 256);
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
	static volatile uint32_t timeout = 1000000000;
	while(timeout) timeout--;
}
void readHymn(void)
{
	int i;
	while(1){
		for(i=10;i<100;i+=5){
			soundTest(i);
			sleep(20000);
		}
		for(i=100;i>10;i-=5){
			soundTest(i);
			sleep(20000);
		}
	}
}
