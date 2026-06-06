/*__attribute__((section(".multiboot")))
unsigned int multiboot_header[] = {
    0x1BADB002,
    0x0,
    -(0x1BADB002)
};
*/
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
	outb(0x3f8,'K');
	load_idt();
	colorize();
	
	//printa("SkyOS 1.0");
	
	//id_mapping();
	*(volatile uint32_t*)0xDEADBEEF = 'f';
	init_keyboard();
	coordinate_print("[SYSTEM] Keyboard Initialized",0,0);
	mouse_init();
	coordinate_print("[SYSTEM] Mouse Initialized",0,1);
	coordinate_print("READY.",0,2);
	//ShellInit();
	init_cursor(0,15);
	update_cursor(0,8);
	outb(0x3d4,0xc);
	
	while(1){asm("HLT");}
}
