#include "vga.h"
#include <stdint.h>

extern void colorize_dbg(void);

void dbg_main(void){
	colorize_dbg();
	print("SkyOS Debugger - Reboot(); for reboot system Dump(); for register dump");
	
}
