#include "keyboard.h"
#include "vga.h"
#include "util.h"
#include "ExternASM.h"
#include "port.h"
#include "speaker.h"
#include "MAllocFree.h"
#define MAX_CHAR 80

extern uint32_t MaxAlloc;
extern bool shift_clicked;
char buf[MAX_CHAR];
int j;

void cmd_fetch()
{
	printk("\n M   M TTTTT  OOO   SSSSS \n");
	printk(" MM MM   T   O   O S      \n");
	printk(" M M M   T   O   O  SSSS  \n");
	printk(" M   M   T   O   O     S  \n");
	printk(" M   M   T    OOO  SSSSS  \n\n");
}

void cmd_info_registers()
{
	uint32_t rbuf[8];
	rbuf[4]=GetIp();
	__asm__ volatile(
		"NOP"
		:"=a"(rbuf[0]),"=b"(rbuf[1]),"=c"(rbuf[2]),"=d"(rbuf[3])
	);
	int xx;
	for(xx=0;xx<5;xx++)
		printk("0x%u\n",rbuf[xx]);
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

void cmd_whereami()
{
	uint32_t stack_frame;
	__asm__ volatile(
		".intel_syntax noprefix\n"
		"LEA EAX,[ESP]\n"
		".att_syntax prefix"
		:"=a"(stack_frame)
	);
	printk("Stack - 0x%u\n",stack_frame);
	printk("Program counter - 0x%u\n",GetIp());
}

void cmd_cls()
{
	struct Cursor CCursor;
	MemSetU16((void*)0xB8000,' '|(COLOR<<8),80*25);
	CCursor.column=0;
	CCursor.line=0;
	update_cursor(CCursor.column,CCursor.line);
}

void cmd_echo(char* buf,int idx)
{
	char temp[60];
	int xx;
	for(xx=0;buf[idx];idx++,xx++)
		temp[xx]=buf[idx];
	temp[xx]='\0';
	printk("%s\n",temp);
}

void cmd_peek(char* buf,int idx)
{
	char temp[32];
	int xx;
	for(xx=0;buf[idx];xx++,idx++)
		temp[xx]=buf[idx];
	temp[xx]='\0';
	uint32_t addres=atoi(temp);
	printk("0x%u\n",*(volatile uint32_t*)addres);
}

void readline()
{
	char c;
	j=0;
	MemSet((void*)buf,0,80);
	shift_clicked=false;
	while(TRUE){
		c=getchar();
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
				break;
			default:
				if(j<MAX_CHAR-1&&c){
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

	if(StrCmp("help",c)==0)
		print("Available commands: help, mtosfetch, whereami, reboot, beep, cls,peek,insmem\n");	
	else if(StrCmp("mtosfetch",c)==0)
		cmd_fetch();
	else if(StrCmp("beep",c)==0)
		Beep(40,80);
	else if(StrCmp("reboot",c)==0)
		cmd_reboot();
	else if(StrCmp("info registers",c)==0)
		cmd_info_registers();
	else if(StrCmp("whereami",c)==0)
		cmd_whereami();
	else if(StrCmp("cls",c)==0)
		cmd_cls();
	else if(StrnCmp("echo",c,3)==0)
		cmd_echo(buf,5);	
	else if(StrnCmp("peek",c,3)==0)
		cmd_peek(buf,5);
	else if(StrCmp("insmem",c)==0){
		printc("$GREEN$Installed memory(MB) -");
		printk("%d\n",*(volatile uint32_t*)0x9000);
	}
	else
		printk("Unknown cmd:%s\n",buf);
}

void term_start()
{
	while(TRUE){
		print("~# ");
		readline();
		if(j>0)
			parse_cmd(buf);
	}
}
