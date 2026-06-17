#include "util.h"
#include <stdint.h>
#include <stddef.h>
#include "vga.h"
#include "externASM.h"
//Random Number Location - 0x8000.
void *kmemcpy(void* From,void* To,uint32_t BitsN){
	uint8_t* f = (uint8_t*)From;
	uint8_t* t = (uint8_t*)To;

	while(BitsN--) *t++ = *f++;
	return To;
}
void* kmemset(void* dest,size_t bytes,size_t fill){
	uint8_t* d = (uint8_t*)dest;
	while(bytes--) *d++ = fill;
	return dest;
}
uint32_t kstrlen(char* buff){
	char* p = buff;

	while(*p != '\0'){
		p++;	
	}
	return (uint32_t)(p - buff);
}
uint32_t kmemcmp(void* destinition,void* src){//compare destinition to src
	uint8_t* d = (uint8_t*)destinition;
	uint8_t* s = (uint8_t*)src;
	while(*d++ && *s++){
		if(*d != *s) return 1;
	}
	return 0;
}
void kmemmove(void* destinition,void* src,uint32_t bits){
	kmemcpy(destinition,src,bits);
	kmemset(destinition,bits,' '|(0x30 << 8));	
}
uint32_t atoi(char buff[]){
	uint32_t n;
	for(uint16_t i = 0; *(buff + i) >= '0' &&  *(buff + i) <= '9';++i){
	 	n = 10 * n + (*(buff + i) - '0');
	}
	return n;
}

void delay(uint32_t huh){
	while(huh--) asm volatile("nop");
}

char* utoa(uint32_t val,uint32_t base){
	static char r[10] = {0};
	static char buff[16] = "0123456789ABCDEF";

	uint32_t n = 8;

	for(; val && n; n--,val /= base)
		r[n] = buff[val % base];
	for(uint32_t i=0;i<n;i++){
		if(r[i] == 0)
			r[i] = '0';
	}
	return &r[n + 1];
}
uint32_t offset(uint32_t** ptr){
	printk("%u",*ptr);
	return (uint32_t)ptr;
}
void ZeroExtend(char* s){
	int i = 0;
	char Temp[10];
	int Size = sizeof(Temp) / sizeof(Temp[0]); 
	while(*s++){
		Temp[i++] = *s;
	}
	int Times = 1024 - Size;
	for(int j = 0;j<Times;j++){
		s[j] = 0;
	}
}
char* rdrand(uint32_t count,uint32_t mod,uint32_t mul,uint32_t dec)
{
	uint32_t randoms[count];
	char* ptr = (char*)randoms;
	for(uint32_t i=1;i<count+1;i++)
		randoms[i]=((randoms[i-1]*mul)+dec)%mod;
	return ptr;
}
uint32_t rdrand_int(uint32_t seed,uint32_t mod,uint32_t mul,uint32_t dec)
{
	return ((seed*mul)+dec)%mod;
}
uint8_t randomU8(void)
{
	rdrandU8();
	return *(uint8_t*)0x8000;
}
uint16_t randomU16(void)
{
	rdrandU16();
	return *(uint16_t*)0x8000;
}
uint32_t randomU32(void)
{
	rdrandU32();
	return *(uint32_t*)0x8000;
}
