#ifndef MALLOCFREE_H
#define MALLOCFREE_H
#include <stdint.h>
void FFHeapInit();
void* FFMalloc(uint32_t request);
void* FFCalloc(uint32_t num,uint32_t size);
#endif
