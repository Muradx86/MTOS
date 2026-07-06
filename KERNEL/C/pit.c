#include <stdint.h>
#include "pit.h"
#include "port.h"
struct Data{
	uint32_t Hour;
	uint32_t Second;
	uint32_t Minute;
	uint32_t Tick;
};
struct Data* Data;
void Set0()
{
	Data->Hour=0;
	Data->Minute=0;
	Data->Second=0;
	Data->Tick=0;
}
void IRQ32Handler()
{
	Data->Tick++;
	if(Data->Tick%100==0){
		Data->Second++;
		if(Data->Second%60==0)
			Data->Minute++;
	}
}
void setFrequency(uint32_t freq)
{
	uint32_t get_freq = FREQUENCY / freq;
	outb(0x43,0x36);
	outb(0x40,(uint8_t)(get_freq));
	outb(0x40,(uint8_t)(get_freq >> 8));
	
}
void sleep(uint32_t microseconds)
{
	while(Data->Tick%(microseconds*100)!=0){
		asm volatile("NOP");
		Data->Tick++;
	}
}
