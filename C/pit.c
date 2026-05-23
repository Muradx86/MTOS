#include <stdint.h>
#include "pit.h"
#include "port.h"

void setFrequency(uint32_t freq){
	uint32_t get_freq = FREQUENCY / freq;
	outb(0x43,0x36);
	outb(0x40,(uint8_t)(get_freq));
	outb(0x40,(uint8_t)(get_freq >> 8));
	
}

void sleep(uint32_t microseconds){
	outb(0x43,0x30);
	outb(0x40,(uint8_t)microseconds);
	outb(0x40,(uint8_t)(microseconds >> 8));
}
