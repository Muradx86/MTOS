#include <stdint.h>
#include <stdbool.h>
typedef double F64;
extern void PIANO_MAIN();
extern void PIANO_DESTROY();
extern void STORE_BACKBUF();
extern void LOAD_SCREEN();
extern void random_hw();
extern void rdrandU8();
extern void rdrandU16();
extern void rdrandU32();
extern void ComplementBit();
extern void testif();
extern void* MemSet(void* Where,uint8_t val,uint32_t Bytes);
extern void* MemSetU16(void* Where,uint16_t val,uint32_t Bytes);
extern void* MemSetU32(void* Where,uint16_t val,uint32_t Bytes);
extern void* MemCpy(void* Where,void* Destinition,uint32_t Bytes);
extern void* MemCpyU16(void* Where,void* Destinition,uint32_t Bytes);
extern void* MemCpyU32(void* Where,void* Destinition,uint32_t Bytes);
extern void* StrCpy(void* dest,char* s);
extern uint32_t StrLen(char* s);
extern char* Reverse(char* s);
extern F64 Sin(F64 x);
extern F64 Cos(F64 x);
extern F64 Sqrt(uint32_t x);
extern uint32_t Round(F64 x);
extern uint32_t ToU32(uint16_t x);
extern uint32_t Log2(uint32_t x);
extern int _Abs(int x);
extern uint32_t Pow(uint32_t x,uint32_t y);
extern void _fast();
extern void _slow();
extern void* PUT_STRING(int x,int y,char* c); //Ptr to first coordinate
extern char* from_above(int len);
extern uint32_t atoi(char* buf);
extern uint8_t Bt(char* dest,uint32_t bit);
extern bool KbdHasKey();
