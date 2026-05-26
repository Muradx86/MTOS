#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
//#define PRESENT 0x1
#define ABSENT 0x0

//page table and page directory both have 1024 entries
uint32_t page_directory[1024]__attribute__((aligned(4096)));
uint32_t page_table[1024]__attribute__((aligned(4096)));

//functions
void id_mapping(void);
extern void enable_paging(void);

#endif
