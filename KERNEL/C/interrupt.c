#include <stdint.h>
#include "vga.h"
#include "MAllocFree.h"
extern void DieScreen();
struct Cursor* CPanicCtrl;
void InterruptHandler(void){
	DieScreen();

	asm volatile("CLI");
	struct gpr{
		uint32_t eip;
		uint32_t esp;
		uint32_t ebp;
		uint32_t cr2; 
		uint32_t eax;
		uint32_t ebx;
		uint32_t ecx;
		uint32_t edx;
		uint32_t edi;
		uint32_t esi;
		uint32_t errcode;
	};

	struct gpr* GeneralRegisters=(struct gpr*)FFMalloc(sizeof(GeneralRegisters));

	GeneralRegisters->eip = *(volatile uint32_t*)0x9000;
	if(GeneralRegisters->eip<0x20000){
		CPanicCtrl->line=0;
		CPanicCtrl->column=20;
		update_cursor(CPanicCtrl->column,CPanicCtrl->line);
		printk("KERNEL IN GARBAGE ZONE\n");
	}
	CPanicCtrl->line=6;
	CPanicCtrl->column=0;
	update_cursor(CPanicCtrl->column,CPanicCtrl->line);
	printk("KERNEL PANIC. REGISTERS:");
	print("\n\n");

	asm volatile
	(".intel_syntax noprefix\n"
	 "MOV %0,ESP\n"
	 ".att_syntax prefix"
	 :"=r"(GeneralRegisters->esp));

	volatile uint32_t* stack_tracer = (volatile uint32_t*)GeneralRegisters->esp;

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

	//My printk() has newline bug so i handcoded it.
	printk("EIP - %u",GeneralRegisters->eip);print("\n");
	printk("EAX - %u",GeneralRegisters->eax);print("\n");
	printk("EBX - %u",GeneralRegisters->ebx);print("\n");
	printk("ECX - %u",GeneralRegisters->ecx);print("\n");
	printk("EDX - %u",GeneralRegisters->edx);print("\n");
	printk("EDI - %u",GeneralRegisters->edi);print("\n");
	printk("ESI - %u",GeneralRegisters->esi);print("\n");
	printk("ESP - %u",GeneralRegisters->esp);print("\n");
	printk("EBP - %u",GeneralRegisters->ebp);print("\n");
	printk("CR2 - %u",GeneralRegisters->cr2);print("\n");
	print("\n");
	printk("Stack Trace: ");print("\n");
	++CPanicCtrl->line;
	update_cursor(CPanicCtrl->column,CPanicCtrl->line);
	printk("%u   %u   %u  %u  %u  %u  %u"
		  ,*(stack_tracer),*(stack_tracer+4),*(stack_tracer-4)
		  ,*(stack_tracer+8),*(stack_tracer-8),*(stack_tracer+12)
		  ,*(stack_tracer-12));

	for(;;)
		asm volatile("HLT");	
	
}
