#ifndef IDT_H
#define IDT_H
#include <stdint.h>
#define PACKED __attribute__((packed))

void pic_remap(void);
void load_idt(void);
void set_gate(uint32_t handler,uint8_t vector);
void enable_interrupts(void);
void disable_interrupts(void);
void idle_state(void);

#endif
