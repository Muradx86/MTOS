#include "port.h"
#include "pit.h"
void SetSound(uint32_t freq)
{
	uint32_t getfreq;
	outb(0x43,0xb6);
	uint8_t c=inb(0x61);
	getfreq=freq/FREQUENCY;
	c|=3;
	outb(0x61,c);
	outb(0x42,getfreq&0xff);
	outb(0x42,getfreq>>8);
}
void StopSound()
{
	uint8_t status=inb(0x61)&0xFC;
	outb(0x61,status);
}
void Beep(uint32_t freq,uint32_t m)
{
	SetSound(freq);
	Sleep(m);
	StopSound();
}
