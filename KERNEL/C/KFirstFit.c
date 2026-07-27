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
	if(request>HEAP_END-HEAP_START||Free->size<request)
		return NULL;
	while(Free){
		if(Free->size>=request&&Free->free){
			Free->free=false;
			return Free;
		}
		Free=Free->next;
	}
}
static void SplitBlk(Blk** HeaderBlk,uint32_t request)
{
	if(!HeaderBlk) //Ignore null.
		return;
	(*HeaderBlk)->next=*HeaderBlk+request;
	(*HeaderBlk)->next->free=true;
	(*HeaderBlk)->next->size=(*HeaderBlk)->size-request;
	(*HeaderBlk)->size=request;
}
void* FFMalloc(uint32_t request)
{	
	if(!request)
		return NULL;
	NewBlk=FindBlk(request);
	if(NewBlk==NULL)
		return NULL;
	SplitBlk(&NewBlk,request);	
	return (void*)((char*)NewBlk+sizeof(NewBlk));
}
void* FFCalloc(uint32_t num,uint32_t size)
{
	if(!num||!size)
		return NULL;
	Blk* Ptr=FFMalloc(num*size);
	Blk* Temp=Ptr+sizeof(Temp);
	if(Ptr)
		KMemset(Temp,0,num*size);
	return (void*)(Ptr);
}
void* FFRealloc(void* ptr,uint32_t newsize)
{//TODO: After FFFree()
	if(!newsize)
		return ptr;
	if(ptr==NULL)
		return NULL;
	Blk* temp=(Blk*)((char*)ptr-sizeof(NewBlk));  //Come to previous struct.
	Blk** ResizeBlk=&temp;
	ResizeBlk=KMemcpy((void*)(*ResizeBlk)->next,
					  (void*)(*ResizeBlk)->next+newsize,
					  sizeof(ResizeBlk));
	(*ResizeBlk)->size=temp->next->size-newsize;
	return NULL; //FIXME		
}
