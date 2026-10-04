#include "util.h"
#include <stdint.h>
#include <stddef.h>
#include "vga.h"
#include "ExternASM.h"
#define RNDM_LOC 0x8000

int Hex2Dec(char* s)
{
	int ret=0,i,digit,len=StrLen(s);
	len--;
	for(i=0;s[i]!='\0';i++,len--){
		if(s[i]>='0'&&s[i]<='9')
			digit=s[i]-'0';
		else if(s[i]>='a'&&s[i]<='f')
			digit=(s[i]-97)+10;
		ret+=digit*Pow(16,len);
	}	
	return ret;
}

int StrCmp(char* dest,char* src)
{
	char* a=dest,*b=src;
	while(*a&&*a==*b){ 
		a++; 
		b++;
	}
	return *a-*b;
}

int StrnCmp(char* dest,char* src,int n)
{
	char* a=dest,*b=src;
	while(*a&&*a==*b&&n){
		a++;
		b++;
		n--;
	}
	return *a-*b;
}

uint32_t Strlen(char* s)
{
	uint32_t ret=1;
	while(*s++)
		ret++;
	return ret;
}

char* utoa(uint32_t val,uint32_t base)
{
	static char r[10] = {0};
	static char buff[16] = "0123456789ABCDEF";

	uint32_t n = 8;

	for(; val&&n; n--,val /= base){
		if(val==0)
			r[n]='0';
		r[n] = buff[val % base];
	}
	return &r[n + 1];
}

uint32_t offset(char** ptr){
	return (uint32_t)ptr;
}
char* rdrand(uint32_t count,uint32_t mod,uint32_t mul,uint32_t dec)
{
	uint32_t randoms[count];
	char* ptr = (char*)randoms;
	for(uint32_t i=0;i<count+1;i++)
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
	return *(uint8_t*)RNDM_LOC;
}
uint16_t randomU16(void)
{
	rdrandU16();
	return *(uint16_t*)RNDM_LOC;
}
uint32_t randomU32(void)
{
	rdrandU32();
	return *(uint32_t*)RNDM_LOC;
}
bool digit(char c)
{
	if(c<='9'&&c>='0')
		return TRUE;
	else
		return FALSE;
}
