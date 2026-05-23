#include "port.h"
#include <stdint.h>
#define PACKED __attribute__((packed));
struct mouse_event{
	uint8_t byte1;
	uint8_t byteX;
	uint8_t byteY;
}PACKED;
static inline void mouse_wait(void){
	uint32_t i = 1000000;
	while(i) i--;
}
void mouse_init(void){
	uint8_t flags;
	outb(0x60,0x60);
	mouse_wait();
	flags |= (1 << 5);
	flags |= (1 << 1);
	outb(0x60,flags);
}
void mouse_handler(struct mouse_event* mouse_event){
	
}
