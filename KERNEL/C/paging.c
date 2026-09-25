#include <stdint.h>
#include "paging.h"
#include <stddef.h>
#include "vga.h"
#define PACKED __attribute__((packed))
#define ALIGNED __attribute__((aligned(4096)))
#define MAX_ENTRY 1024
struct pde{
	uint8_t present;			
	uint8_t rw;
	uint8_t us;
	uint8_t pwt;
	uint8_t pcd;
	uint8_t a;
	uint8_t d;
	uint8_t ps;
	uint8_t g;
	uint8_t avl;
	uint8_t pat;
	uint8_t bits3932;
	uint8_t rsvd;
	uint16_t bits3122;
}PACKED;

struct pde pde_t[MAX_ENTRY] ALIGNED;

//struct 'phy' for physical addres layout
struct phy{
	uint16_t dir;
	uint32_t offset;
	uint8_t Phy3932;
}PACKED;

struct phy physical_addres;


void id_mapping(void){


	asm volatile(".intel_syntax noprefix\n"
					 "MOV CR3,%0\n" 
				".att_syntax prefix"
				 :
				 :"r"(&pde_t)
				);

	enable_paging();
}
