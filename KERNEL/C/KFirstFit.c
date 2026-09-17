#include "MAllocFree.h"
#include "vga.h"
#include "ExternASM.h"

#define NULL	   (void(*))(0)
#define HEAP_START 0x500000
#define HEAP_END   0x900000
uint32_t MaxAlloc;
typedef struct Block{
	bool free;
	uint32_t size;
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
	MaxAlloc=0;
}

static Blk* FindBlk(uint32_t request)
{
	if(request>HEAP_END-HEAP_START||Free->size<request)
		return NULL;
	while(Free){
		if(Free->size>=request&&Free->free)
			return Free;
		Free=Free->next;
	}
	return NULL;
}

static void ModifyBlk(Blk** HeaderBlk,uint32_t request)
{
	if(!HeaderBlk)
		return;
	(*HeaderBlk)->next=*(HeaderBlk)+request;
	(*HeaderBlk)->next->free=true;
	(*HeaderBlk)->next->size=(*HeaderBlk)->size-request;
	(*HeaderBlk)->size=request;
	(*HeaderBlk)->free=false;
}

void* FFMalloc(uint32_t request)
{	
	if(!request)
		return NULL;
	NewBlk=FindBlk(request);
	if(NewBlk==NULL)
		return NULL;
	ModifyBlk(&NewBlk,request);	
	MaxAlloc+=request;
	return (void*)((char*)NewBlk+sizeof(NewBlk)+sizeof(NewBlk->next));
}

void* FFCalloc(uint32_t num,uint32_t size)
{
	if(!num||!size)
		return NULL;
	Blk* Ptr=FFMalloc(num*size);
	Blk* Temp=(Blk*)((char*)Ptr+sizeof(Temp)+sizeof(Temp->next));
	if(Ptr)
		MemSet(Temp,0,num*size);
	else
		return NULL;
	return (void*)(Temp);
}

void* FFRealloc(void* ptr,uint32_t newsize)
{//TODO: After FFFree()
	if(!newsize)
		return ptr;
	if(ptr==NULL)
		return NULL;
	Blk* temp=(Blk*)((char*)ptr-sizeof(NewBlk));  //Come to previous struct.
	Blk** ResizeBlk=&temp;
	ResizeBlk=MemCpy((void*)(*ResizeBlk)->next,
					  (void*)(*ResizeBlk)->next+newsize,
					  sizeof(ResizeBlk));
	(*ResizeBlk)->size=temp->next->size-newsize;
	return NULL; //FIXME		
}

void FFFree(void* ptr)
{
	Blk* RootPtr=ptr-sizeof(RootPtr)-sizeof(RootPtr->next);  //Get root access on ptr.
	if(RootPtr->free==true||!ptr)
		return;
	RootPtr->next=RootPtr->next->next;
	RootPtr->free=true;
}
