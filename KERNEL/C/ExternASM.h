#include <stdint.h>
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
extern void* KMemset(void* Where,uint8_t val,uint32_t Bytes);
extern void* KMemcpy(void* Where,void* Destinition,uint32_t Bytes);
