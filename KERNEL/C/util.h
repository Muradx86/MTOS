#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
//memory
uint32_t kmemcmp(void* destinition,void* src);
void kmemmove(void* destinition,void* src,uint32_t bits);

//string releated
uint32_t atoi(char* buf);
char* utoa(uint32_t val,uint32_t base);//base is 16 always
int StrCmp(char* dest,char* src);
//misc
void ZeroExtend(char* s);
void delay(uint32_t huh);
void* reverse(char* s);
uint32_t offset(char** ptr);
char* rdrand(uint32_t count,uint32_t mod,uint32_t mul,uint32_t dec);
uint32_t rdrand_int(uint32_t seed,uint32_t mod,uint32_t mul,uint32_t dec);
uint8_t real_random(void);
bool digit(char c);
uint8_t randomU8(void);
uint16_t randomU16(void);
uint32_t randomU32(void);
#define FLAG_CARRY 		(1<<0)
#define FLAG_DIRECTION  (1<<10)
#define FLAG_SIGN		(1<<7)
#define FLAG_INTERRUPT	(1<<9)
#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0
#endif
