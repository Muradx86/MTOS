#include "port.h"
#include <stdint.h>
#define PACKED __attribute__((packed));
union MouseStatus{
	uint32_t all : 24;
};
void mouse_handler(){
	uint8_t a = inb(0x60);
	
	return;
}
