#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>

//memory
void* kmemset(void* dest,size_t bytes,size_t fill);
void *kmemcpy(void* From,void* To,uint32_t BitsN);
uint32_t kmemcmp(void* destinition,void* src);

//string releated
uint32_t kstrlen(char* buff);
uint32_t atoi(char num[]);
char* utoa(uint32_t val,uint32_t base);//base is 16 always

//misc
void ZeroExtend(char* s);
void delay(uint32_t huh);
void* reverse(char* s);
uint32_t* offset(uint32_t** ptr);
#endif
