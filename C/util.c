#include "util.h"
#include <stdint.h>
#include <stddef.h>
#include "vga.h"
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

uint32_t atoi(char buff[]){
	uint32_t n;
	for(uint16_t i = 0; *(buff + i) >= '0' &&  *(buff + i) <= '9';++i){
	 	n = 10 * n + (*(buff + i) - '0');
	}
	return n;
}

void delay(uint32_t huh){
	while(huh) asm volatile("nop");
}

char* utoa(uint32_t val,uint32_t base){
	//uint32_t base = 16;
	static char r[10] = {0};
	static char buff[16] = "0123456789ABCDEF";

	uint32_t n = 8;


	for(; val && n; n--,val /= base){
		r[n] = buff[val % base];
	}

	return &r[n + 1];
}

void *kmemcpy(void* From,void* To,uint32_t BitsN){
	uint8_t* f = (uint8_t*)From;
	uint8_t* t = (uint8_t*)To;

	while(BitsN--) *t++ = *f++;
	return To;
}
uint32_t* offset(uint32_t** ptr){
	printk("%u",*ptr);
	return *(uint32_t*)ptr;
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
