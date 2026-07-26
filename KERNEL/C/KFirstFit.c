#include "MAllocFree.h"
#include "ExternASM.h"
#include <stdbool.h>
#define NULL	   (void*)0
#define HEAP_START 0x500000
#define HEAP_END   0x900000
typedef struct Block{
	uint32_t size;
	bool free;
	struct Block* next;
}Blk;
Blk* Free;
Blk* NewBlk;
void FFHeapInit()
{
	Blk* MemStart=(Blk*)HEAP_START;
	MemStart->free=true;
	MemStart->size=HEAP_END-HEAP_START;
	MemStart->next=NULL;
	Free=MemStart;
}
static Blk* FindBlk(uint32_t request)
{
	if(request>HEAP_END-HEAP_START)
		return NULL;
	while(Free){
		if(Free->size>request&&Free->free){
			Free->free=false;
			return Free;
		}
		Free=Free->next;
	}	
}
static void SplitBlk(Blk** HeaderBlk,uint32_t request)
{
	(*HeaderBlk)->next=*HeaderBlk+request;
	(*HeaderBlk)->next->free=true;
	(*HeaderBlk)->next->size=(*HeaderBlk)->size-request;
	(*HeaderBlk)->size=request;
}
void* FFMalloc(uint32_t request)
{	
	NewBlk=FindBlk(request);
	if(NewBlk==NULL)
		return NULL;
	SplitBlk(&NewBlk,request);	
	return (void*)((char*)NewBlk+sizeof(NewBlk));
}
void* FFCalloc(uint32_t num,uint32_t size)
{
	uint8_t* Ptr=(uint8_t*)FFMalloc(num*size);
	uint32_t temp;
	for(temp=0;temp<size;temp++)
		*(Ptr+temp)='A';
	return (void*)(Ptr);
}
