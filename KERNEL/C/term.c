#include "keyboard.h"
#include "vga.h"
#include "util.h"
#include "ExternASM.h"
#include "port.h"
#include "speaker.h"
#define MAX_CHAR 80

char buf[MAX_CHAR];
char aux_buf[MAX_CHAR];
int j;
int k;

void cmd_fetch()
{
	print(" M   M TTTTT  OOO   SSSSS \n");
	print(" MM MM   T   O   O S      \n");
	print(" M M M   T   O   O  SSSS  \n");
	print(" M   M   T   O   O     S  \n");
	print(" M   M   T    OOO  SSSSS  \n");
}

void cmd_reboot()
{
	void* ptr_destroy;
	__asm__ volatile(
		".intel_syntax noprefix\n"
		"SIDT [EBP-64]\n"
		"MOV DWORD PTR [EBP-64],0xDEAD\n"
		"LIDT [EBP-64]\n"
		".att_syntax prefix"
		:"=a"(ptr_destroy)
	);
	MemSetU32(ptr_destroy,0xDEAD,80);
	__asm__ volatile("INT $0");
}

uint32_t parse_hex(char* s)
{
	char hex_buf[]={0};
	int i=2,j=0;
	if(s[0]!='0'&&s[1]!='x')
		return 0;
	while(s[i])
		hex_buf[j++]=s[i++];
	return	atoi(utoa(atoi(hex_buf),10));	
}

void readline()
{
	char c;
	j=0,k=0;
	MemSet((void*)buf,0,80);
	while(TRUE){
		c=get_char();
		switch(c){
			case '\n':
				vga_putchar('\n');
				return;
			case '\b':
				vga_putchar('\b');
				buf[j]=0;
				j--;
				break;
			case '\t':
				vga_putchar('\t');
				while(c!='\n'){
					vga_putchar(c);
					aux_buf[k++]=c;
				}
				break;
			default:
				if(j<MAX_CHAR-1&&c<127&&!(c&0x80)){
					vga_putchar(c);
					buf[j++]=c;
				}
				break;
		}
	}
}

void parse_cmd(char* c)
{
	c[j]=0;
	if(StrCmp("dddd",c)==0)
		print("DIR;\n");
	else if(StrCmp("fetch",c)==0)
		cmd_fetch();
	else if(StrCmp("beep",c)==0)
		Beep(50,50);
	else if(StrCmp("rebt",c)==0)
		cmd_reboot();
	else if(StrCmp("peek",c)==0&&StrLen(aux_buf)!=0){
		*(volatile uint16_t*)0xb8014a='J'|(COLOR<<8);
		printk("auxbuf-%s\n",aux_buf);
	}
	else
		printk("Unknown cmd:%s\n",buf);
}

void term_start()
{
	while(TRUE){
		print("~> ");
		readline();
		if(j>0)
			parse_cmd(buf);
	}
}
