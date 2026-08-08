#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <stdint.h>
void init_keyboard(void);
void keyboard_handler(void);
void queu_start();
char get_char();
#endif
