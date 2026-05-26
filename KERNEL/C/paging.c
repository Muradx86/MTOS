#include <stdint.h>
#include "paging.h"
#include <stddef.h>
#include "vga.h"
#define PACKED __attribute__((packed))
#define ALIGNED __attribute__((aligned(4096)))
#define MAX_ENTRY 1024
struct pde{
	uint8_t present : 1;			
	uint8_t rw : 1;
	uint8_t us : 1;
	uint8_t pwt : 1;
	uint8_t pcd : 1;
	uint8_t a :1;
	uint8_t d : 1;
	uint8_t ps : 1;
	uint8_t g : 1;
	uint8_t avl : 3;
	uint8_t pat : 1;
	uint8_t bits3932;
	uint8_t rsvd : 1;
	uint16_t bits3122 : 10;
}PACKED;

struct pde pde_t[MAX_ENTRY] ALIGNED;

//struct 'phy' for physical addres layout
struct phy{
	uint16_t dir : 10;
	uint32_t offset : 22;
	uint8_t Phy3932;
}PACKED;

struct phy physical_addres;


void id_mapping(void){

	uint32_t RealAddres = 0;
	
	for(uint16_t idx = 0; idx < 1025;idx++,RealAddres += 4000000){//LOOP is entirely WRONG!
		physical_addres.offset = RealAddres & 0x0011FFFFF;
		physical_addres.Phy3932 = RealAddres & 0xFF000000;
		physical_addres.dir = RealAddres & 0xFF110000;
		
		pde_t[idx].present = 1;
		pde_t[idx].rw = 1;
		pde_t[idx].us = 1;
		pde_t[idx].pwt = 1;
		pde_t[idx].pcd = 0;
		pde_t[idx].a = 0;
		pde_t[idx].d = 0;
		pde_t[idx].ps = 1;
		pde_t[idx].g = 0;
		pde_t[idx].avl = 0;
		pde_t[idx].pat = 0;
		pde_t[idx].bits3932 = RealAddres & 0xFF000000;
		pde_t[idx].rsvd = 0;
		pde_t[idx].bits3122 = RealAddres & 0xFF110000;
	}

	//PDE
		pde_t[0].present = 1;
		pde_t[0].rw = 1;
		pde_t[0].us = 1;
		pde_t[0].pwt = 1;
		pde_t[0].pcd = 0;
		pde_t[0].a = 0;
		pde_t[0].d = 0;
		pde_t[0].ps = 1;
		pde_t[0].g = 0;
		pde_t[0].avl = 0;
		pde_t[0].pat = 0;
		pde_t[0].bits3932 = RealAddres & 0xFF000000;
		pde_t[0].rsvd = 0;
		pde_t[0].bits3122 = RealAddres & 0xFF110000;

	asm volatile(".intel_syntax noprefix\n"
					 "MOV CR3,%0\n" 
				".att_syntax prefix"
				 :
				 :"r"(&pde_t)
				);

	enable_paging();
	coordinate_print("[SYSTEM] Paging Activated",0,1);	
}
