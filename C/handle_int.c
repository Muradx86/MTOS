#include <stdint.h>
#include "vga.h"
#include "util.h"
#define PACKED __attribute__((packed))
extern void DieScreen();
extern void assembly_helper();
extern void assembly_helper_err();


void InterruptHandler(void){
	DieScreen();
	asm volatile("CLI");
	
	struct gpr{
		volatile uint32_t eip;
		volatile uint32_t esp;
		volatile uint32_t ebp;
		volatile uint32_t cr2; 
		volatile uint32_t eax;
		volatile uint32_t ebx;
		volatile uint32_t ecx;
		volatile uint32_t edx;
		volatile uint32_t edi;
		volatile uint32_t esi;
		volatile uint32_t errcode;
	}PACKED;

	struct gpr* GeneralRegisters;

	uint32_t eiptmp = *(uint32_t*)0x9000;
	GeneralRegisters->eip = eiptmp;

	printk("KERNEL PANIC. REGISTERS:");
	printa("\n");
	printa("\n");

	asm volatile(".intel_syntax noprefix\n"
				 "MOV %0,ESP\n"
				 ".att_syntax prefix"
				 :"=r"(GeneralRegisters->esp));

	uint32_t* stack_tracer = (uint32_t*)GeneralRegisters->esp;

	asm volatile(".intel_syntax noprefix\n"
				"MOV %0,EAX\n"
				".att_syntax prefix"
				 :"=r"(GeneralRegisters->eax));

	asm volatile(".intel_syntax noprefix\n"
				 "MOV %0,EBX\n"
				 ".att_syntax prefix"
				 :"=r"(GeneralRegisters->ebx));

	asm volatile(".intel_syntax noprefix\n"
				 "MOV %0,ECX\n"
				 ".att_syntax prefix"
				 :"=r"(GeneralRegisters->ecx));

	asm volatile(".intel_syntax noprefix\n"
				 "MOV %0,EDX\n"
				 ".att_syntax prefix"
				 :"=r"(GeneralRegisters->edx));
	
	asm volatile(".intel_syntax noprefix\n"
				"MOV %0,EDI\n"
				".att_syntax prefix"
				:"=r"(GeneralRegisters->edi));
	asm volatile(".intel_syntax noprefix\n"
				"MOV %0,ESI\n"
				".att_syntax prefix"
				:"=r"(GeneralRegisters->esi));

	asm volatile(".intel_syntax noprefix\n"
				 "MOV %0,CR2\n"
				 ".att_syntax prefix"
				 :"=r"(GeneralRegisters->cr2));

	asm volatile(".intel_syntax noprefix\n"
				 "MOV %0,EBP\n"
				 ".att_syntax prefix"
				 :"=r"(GeneralRegisters->ebp));
				 			 
	
	printk("EIP - %u",GeneralRegisters->eip);printa("\n");
	printk("EAX - %u",GeneralRegisters->eax);printa("\n");
	printk("EBX - %u",GeneralRegisters->ebx);printa("\n");
	printk("ECX - %u",GeneralRegisters->ecx);printa("\n");
	printk("EDX - %u",GeneralRegisters->edx);printa("\n");
	printk("EDI - %u",GeneralRegisters->edi);printa("\n");
	printk("ESI - %u",GeneralRegisters->esi);printa("\n");
	printk("ESP - %u",GeneralRegisters->esp);printa("\n");
	printk("EBP - %u",GeneralRegisters->ebp);printa("\n");
	printk("CR2 - %u",GeneralRegisters->cr2);printa("\n");
	printa("\n");
	printk("Stack Trace: ");printa("\n");
	printk("&ESP - %u",*(stack_tracer));printa("\n");
	printk("&ESP + 4 - %u",*(stack_tracer + 4));printa("\n");
	printk("&ESP - 4 - %u",*(stack_tracer - 4));printa("\n");
	printk("&ESP + 8 - %u",*(stack_tracer + 8));printa("\n");
	printk("&ESP - 8 - %u",*(stack_tracer - 8));printa("\n");
	printk("&ESP + 12 - %u",*(stack_tracer + 12));printa("\n");
	printk("&ESP - 12 - %u",*(stack_tracer - 12));printa("\n");
	printk("&ESP + 16 - %u",*(stack_tracer + 16));printa("\n");
	printk("&ESP - 16 - %u",*(stack_tracer - 16));printa("\n");
	printk("&ESP + 20 - %u",*(stack_tracer + 20));printa("\n");
	printk("&ESP - 20 - %u",*(stack_tracer - 20));printa("\n");

	for(;;)
		asm volatile("HLT");	
}

void IRQ32Handler(void){
	uint32_t CurrentTickNum = 0;
	//print_clock(CurrentTickNum / 18);
	CurrentTickNum++;
}
