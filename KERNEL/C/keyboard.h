#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>
void init_keyboard(void);
void keyboard_handler(void);
extern char FIFOBuf[80];
#endif
