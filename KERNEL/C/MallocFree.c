#include "vga.h"
#define NULL       (void(*))(0)
#define HEAP_START 0x500000
#define HEAP_END   0x900000
#define FREE       0xF4EE
#define USED       0x45ED
typedef struct Blk{
	int flag;	
	int size;
	struct Blk* next;
}Blk;
static int Allocated;
Blk* MemStart;
Blk* First;//List of available space.
void MemoryInit()
{
	MemStart=(Blk*)HEAP_START;
	MemStart->size=HEAP_END-HEAP_START;
	MemStart->flag=FREE;
	MemStart->next=NULL;
	Allocated=0;
	First=MemStart;
}
static void DivBlk(Blk* Block,int request)
{
	Block->size=Block->size-request;
	Block->next=Block+request; //Point to next free blk.
	Block->flag=FREE;
} 
static Blk* FindFree(int request)
{//Algorithm uses first fit method.
	Blk* Fit;
	if(request>=HEAP_END-HEAP_START||First->size<=request){
		Fit=NULL;
		return Fit;
	}
	while(First){
		if(First->flag==FREE&&First->size>=request){
			Fit=First;
			return Fit;
		}
		First=First->next;
	}//Algorithm will terminate.
	print("Allocation failed.\n");
	return NULL;
}
void* KMalloc(int request)
{
	if(request<=0)
		return NULL;
	First=FindFree(request);
	if(First==NULL){
		printk("Allocation failed\n");
		return NULL;
	}
	printk("\nbefore->%u\n",First->size);
	DivBlk(First,request);
	printk("\nAfter->%u\n",First->size);
	printk("-------------------------\n");
	return First;
}
