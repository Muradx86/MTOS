#include "assembly.h"
#include <stdint.h>
uint64_t rdtsc()
{
	uint32_t eax,edx;
	asm volatile
	(
		"rdtsc"
		:"=a"(eax),"=d"(edx)
	);
	return ((uint64_t)edx<<32)|eax;
}
void cpuid(uint32_t code,uint32_t* eax,uint32_t* edx,uint32_t* ecx)
{
	asm volatile
	(
		"cpuid"
		:"=a"(*eax),"=d"(*edx),"=c"(*ecx):"0"(code):"ebx"
	);
}
