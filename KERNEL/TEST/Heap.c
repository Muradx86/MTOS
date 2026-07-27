#include "../C/MAllocFree.h"
#include "../C/ExternASM.h"
#include "../C/vga.h"
void alloc_test()
{
	char* ptr=(char*)FFMalloc(10);
	printk("hello from test->%u\n",(uint32_t)ptr);
	char* ptr2=(char*)FFCalloc(10,1);
	printk("second test->%u\n",(uint32_t)ptr2);
	char* ptr3=(char*)FFCalloc(10,1);
	printk("third test->%u\n",(uint32_t)ptr3);
	char* ptr4=(char*)FFCalloc(10,1);
	printk("forth test->%u\n",(uint32_t)ptr4);
}
