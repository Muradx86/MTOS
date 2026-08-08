#include "idt.h"
#include <stdint.h>
#include <stdbool.h>
#include "vga.h"
#include "pit.h"
#include "port.h"
#include "speaker.h"
#include "util.h"
#include "keyboard.h"
#include "graphics.h"
#include "ExternASM.h"
#include "MAllocFree.h"
extern void id_mapping(void);
extern void colorize(void);
extern void testif(void);
extern void enable_paging(void);
extern void keyboard_init();
extern void shell_init(void);
extern void mouse_init(void);
extern void alloc_test();
void kernel()
{//The kernel is a fun zone also.
	asm volatile
	(".intel_syntax noprefix\n"
	 "MOV EAX,CR0\n"
	 "BTC EAX,5\n"
	 "MOV CR0,EAX\n"
	 "FNINIT\n"
	 ".att_syntax prefix"
	);
	outb(0x3f8,'H');
	load_idt();
	Set0();
	SetFrequency(100);
	colorize();
	keyboard_init();
	//id_mapping();
	*(volatile uint64_t*)0xffffffffffffffff = 'f';
	print("------------------------------MTOS initialized!-----------------------------------\n");
	//mouse_init();
	print("----------------------------Welcome MTOS 32-bit OS------------------------------\n");
	init_cursor(2,30);
	update_cursor(0,2);
	outb(0x3d4,0xc);
	FFHeapInit();
	queu_start();
	__asm__("int $8");
	while(1) asm volatile("HLT");
}
