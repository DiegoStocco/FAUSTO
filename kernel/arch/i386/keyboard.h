#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

void handle_scancode(uint8_t scancode);
char translate_scancode(uint8_t scancode);

void kb_buffer_push(char c);
char kb_buffer_pop(void);

char getchar(void);
#endif
