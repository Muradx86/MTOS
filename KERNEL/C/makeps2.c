#include <stdint.h>
#include "port.h"
#define PS2_SC 0x60
#define PS2_R 0x64
static inline void wait(){
	uint64_t wait = 10000000;
	while(inb(PS2_R) & 2 && wait);
}
void mouse_init(void){
	uint8_t flags;
	//outb(0x60,0x60);
	wait();
	flags |= (1 << 5);
	flags |= (1 << 1);
	outb(0x60,flags);
}
void init_keyboard(void)
{
	uint8_t flags;

	wait();
	outb(PS2_SC,0x60);
	wait();
	flags |= (1 << 0);
	flags = ~(1 << 1); 
	flags |= (1 << 6);
	wait();
	outb(PS2_R,flags);
}
