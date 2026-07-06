#include "idt.h"
#include <stdint.h>
#include "vga.h"
#include "port.h"
#include "pit.h"
#include "speaker.h"
#include "util.h"
#include "graphics.h"
#include "ExternASM.h"
extern void id_mapping(void);
extern void colorize(void);
extern void testif(void);
extern void enable_paging(void);
extern void init_keyboard();
extern void shell_init(void);
extern void mouse_init(void);
extern void PMActive(void);
extern uint8_t getch();
extern void Destroy();
extern void rom2ram();
#undef NULL
#define NULL (void *)(0)
void kernel()
{//The kernel is a fun zone also.
	asm volatile
	(".intel_syntax noprefix\n"
	 "MOV EAX,CR0\n"
	 "BTC EAX,5\n"
	 "MOV CR0,EAX\n"
	 "FINIT\n"
	 ".att_syntax prefix"
	);
	setFrequency(100);
	outb(0x3f8,'H');
	load_idt();
	Set0();
	colorize();
	init_keyboard();
	//id_mapping();
	*(volatile uint8_t*)0xffffffffffff = 'f';
	print("------------MTOS initialized!-----------\n");
	mouse_init();
	print("------------Welcome MTOS 32-bit OS------\n");
	init_cursor(2/*1*/,/*14*/20);
	update_cursor(0,3);
	outb(0x3d4,0xc);
	while(1) asm volatile("HLT");
}
