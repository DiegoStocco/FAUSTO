#include <stdint.h>
#include "keyboard.h"
#include <kernel/log.h>
#include <stdio.h>

void handle_scancode(uint8_t scancode) {
	/*if (scancode == 0x10) {
		log_msg(LOG_DEBUG, "keyboard", "q");
	}*/
	printf("%x", scancode);
}
