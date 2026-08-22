#include "keyboard.h"
#include "util.h"
#include "vga.h"
#include "MAllocFree.h"
#define CMD_MAX 80
void term_start()
{
	volatile char* term_buf=(volatile char*)FFCalloc(80,1);
	char c;
	int i=0;

	while(TRUE){
		while((c=get_char())!='\n'){
			term_buf[i]=c;
			i++;
		}
		term_buf[i]='\0';
		print("I am here\n");
		if(kstrcmp("hello",(char*)term_buf)==0)
			printk("Hello world\n");
		i=0;
	}		
}
