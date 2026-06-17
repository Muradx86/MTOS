#include "util.h"
#include "vga.h"
void IRQ32Handler(void){
	static uint32_t Tick = 0;
	static uint32_t Minute = 0;
	static uint32_t Second = 0;
	Tick++;
	if(Tick == 18){
		Tick = 0;
		Second++;
		if(Second == 60){
			Minute++;
			Second = 0;
		}
		//printk(" %d/%d ",Minute,Second);
	}
}
