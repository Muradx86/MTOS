__attribute__((section(".multiboot")))
unsigned int multiboot_header[] = {
    0x1BADB002,
    0x0,
    -(0x1BADB002)
};

// 8/02/2026 - first worked

/* VGA addres calculation - *(vga + width * y + x)*/

#include "idt.h"
#include <stdint.h>
#include "vga.h"
#include "port.h"
#include "pit.h"
#include "speaker.h"
//#include "keyboard.h"
#include "util.h"
#include "graphics.h"
#include "externASM.h"
extern void id_mapping(void);
extern void colorize(void);
extern void testif(void);
extern void enable_paging(void);
extern void init_keyboard();
extern void ShellInit(void);
extern void mouse_init(void);
extern void PMActive(void);

void kernel(void){//The kernel is a fun zone also.
	//PMActive();
	load_idt();
	coordinate_print("[SYSTEM] System initialized",0,2);
	
	colorize();
	//id_mapping();
	*(volatile uint32_t*)0xDEADBEEF = 'A';
	init_keyboard();
	//mouse_init();
	//ShellInit();
	//printk("hello! %u",(uint32_t)0x123);
	/*asm volatile(".intel_syntax noprefix\n"
				 "int 14\n"
				 ".att_syntax prefix");
	*/
	//DRAW_LINE();
	while(1){}
}
