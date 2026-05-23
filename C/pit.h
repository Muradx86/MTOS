#ifndef PIT_H
#define PIT_H

#include <stdint.h>
#define FREQUENCY 1193182			//~1 mhz
#define C0 0x40
#define C1 0x41
#define C2 0x42

void setFrequency(uint32_t freq);
void sleep(uint32_t milliseconds);

#endif
