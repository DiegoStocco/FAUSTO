#include <stdint.h>
#include <kernel/keyboard.h>
#include <kernel/log.h>
#include <stdio.h>
#include <kernel/tty.h>


const char ps2_set1_normal[256] = {
    [0x0B] = '0', [0x02] = '1', [0x03] = '2', [0x04] = '3',
    [0x05] = '4', [0x06] = '5', [0x07] = '6', [0x08] = '7',
    [0x09] = '8', [0x0A] = '9',

    [0x1E] = 'a', [0x30] = 'b', [0x2E] = 'c', [0x20] = 'd',
    [0x12] = 'e', [0x21] = 'f', [0x22] = 'g', [0x23] = 'h',
    [0x17] = 'i', [0x24] = 'j', [0x25] = 'k', [0x26] = 'l',
    [0x32] = 'm', [0x31] = 'n', [0x18] = 'o', [0x19] = 'p',
    [0x10] = 'q', [0x13] = 'r', [0x1F] = 's', [0x14] = 't',
    [0x16] = 'u', [0x2F] = 'v', [0x11] = 'w', [0x2D] = 'x',
    [0x15] = 'y', [0x2C] = 'z',

    [0x29] = '`', [0x0C] = '-', [0x0D] = '=', [0x1A] = '[',
    [0x1B] = ']', [0x2B] = '\\', [0x27] = ';', [0x28] = '\'',
    [0x33] = ',', [0x34] = '.', [0x35] = '/',

    [0x39] = ' ',   // Space
    [0x1C] = '\n',  // Enter
    [0x0E] = '\b',  // Backspace
    [0x0F] = '\t',  // Tab
    [0x01] = 0x1B,  // Escape
};

const char ps2_set1_shifted[256] = {
    [0x0B] = ')', [0x02] = '!', [0x03] = '@', [0x04] = '#',
    [0x05] = '$', [0x06] = '%', [0x07] = '^', [0x08] = '&',
    [0x09] = '*', [0x0A] = '(',

    [0x1E] = 'A', [0x30] = 'B', [0x2E] = 'C', [0x20] = 'D',
    [0x12] = 'E', [0x21] = 'F', [0x22] = 'G', [0x23] = 'H',
    [0x17] = 'I', [0x24] = 'J', [0x25] = 'K', [0x26] = 'L',
    [0x32] = 'M', [0x31] = 'N', [0x18] = 'O', [0x19] = 'P',
    [0x10] = 'Q', [0x13] = 'R', [0x1F] = 'S', [0x14] = 'T',
    [0x16] = 'U', [0x2F] = 'V', [0x11] = 'W', [0x2D] = 'X',
    [0x15] = 'Y', [0x2C] = 'Z',

    [0x29] = '~', [0x0C] = '_', [0x0D] = '+', [0x1A] = '{',
    [0x1B] = '}', [0x2B] = '|', [0x27] = ':', [0x28] = '"',
    [0x33] = '<', [0x34] = '>', [0x35] = '?',

    [0x39] = ' ',   // Space
    [0x1C] = '\n',  // Enter
    [0x0E] = '\b',  // Backspace
    [0x0F] = '\t',  // Tab
    [0x01] = 0x1B,  // Escape
};

#define KEYBOARD_BUFFER_SIZE 256

typedef struct {
	char data[KEYBOARD_BUFFER_SIZE];
	unsigned int head;
	unsigned int tail;
} keyboard_buffer_t;

static keyboard_buffer_t kb_buffer = { .head = 0, .tail = 0 };

void kb_buffer_push(char c) {
	unsigned int next = (kb_buffer.head + 1) % KEYBOARD_BUFFER_SIZE;

	if (next != kb_buffer.tail) {
		kb_buffer.data[kb_buffer.head] = c;
		kb_buffer.head = next;
	}
}

char kb_buffer_pop(void){
	if (kb_buffer.head == kb_buffer.tail) {
		return 0;
	}
	char c = kb_buffer.data[kb_buffer.tail];
	kb_buffer.tail = (kb_buffer.tail + 1) % KEYBOARD_BUFFER_SIZE;
	return c;
}

int shift_pressed = 0;

char translate_scancode(uint8_t scancode) {
	if (shift_pressed) return ps2_set1_shifted[scancode];
	else return ps2_set1_normal[scancode];
}

void handle_scancode(uint8_t scancode) {
	int is_break = (scancode & 0x80) != 0;
	uint8_t clean_scancode = scancode & 0x7F;
	
	// Shift handling
	if (clean_scancode == 0x2A || clean_scancode == 0x36) {
		shift_pressed = !is_break;
		return;
	}
	
	if (is_break) return;
	char c = translate_scancode(clean_scancode);
	kb_buffer_push(c);
}

char getchar(void) {
    char c = 0;
    while ((c = kb_buffer_pop()) == 0) {
        asm volatile("hlt");
    }
    return c;
}
