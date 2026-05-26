#include "keyboard.h"
#include <stdint.h>
#include "port.h"
#include "util.h"
#include "vga.h"
extern char FIFOBuf[80];
char GetChar(void){
	char tmp = *(volatile char*)0x9000;
	return tmp;
}
void BusyWait(void){
	static uint32_t A = 100000;
	while(A--);
}
void fMOVZX(char* s){
	uint16_t Null2Adr = 0;
	while(*s++)
		Null2Adr++;
	uint16_t Dif = 80 - Null2Adr;
	while((s[80 - Dif++] = 0));
}
void ShellInit(){
	char* first = "hello";
	fMOVZX(first);
	char CmdLine[80];
	uint32_t Status;
	static volatile uint16_t idx = 0;
	while(1){
		if(~(inb(0x64) & 2))
			BusyWait();
		if(GetChar() == '\n'){
			CmdLine[idx] = '\0';
			fMOVZX(CmdLine);
			Status = kmemcmp(CmdLine,first);
			if(Status == 0){
				print_shell("HEY");
			}
		}
		CmdLine[idx++] = GetChar();
	}
}
