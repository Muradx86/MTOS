#include <stdint.h>
#define PACKED __attribute__((packed));
void DrawCube(uint8_t x1,uint8_t x2,uint8_t y1,uint8_t y2){//x1 > x2 always
	volatile uint16_t* vga_bufer = (volatile uint16_t*)0xb8000;
	vga_bufer[80*y1 + x1] = 'P' | (0x40 << 8);
	vga_bufer[80*y2 + x2] = 'A' | (0x40 << 8);
}
