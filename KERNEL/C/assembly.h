#ifndef ASSEMBLY_H
#define ASSEMBLY_H
#include <stdint.h>
uint64_t rdtsc();
void cpuid(uint32_t code,uint32_t* eax,uint32_t* edx,uint32_t* ecx);
#endif
