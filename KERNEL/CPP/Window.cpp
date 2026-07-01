#include <stdint.h>
#include <stdbool.h>
class Screen{
	public:
		uint32_t x,y;
};
volatile uint16_t* buffer=(volatile uint16_t*)0xB8000;
Screen* Window;
void print(char* s,bool locked=0)
{
	while(*s++)
		buffer[80*Window->y+Window->x]=*s | (0x30<<8);
}
