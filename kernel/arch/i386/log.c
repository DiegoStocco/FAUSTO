#include "vga.h"
#include <kernel/log.h>
#include <kernel/tty.h>

__attribute__((noreturn))
void kernel_panic(const char *msg)
{
	terminal_writestring("\n\nKERNEL PANIC: ");
	terminal_writestring(msg);
	terminal_putchar('\n');

	asm volatile ("cli");

	for (;;) {
		asm volatile ("hlt");
	}
}


char* get_prefix_from_loglvl(int log_lvl) {
	switch (log_lvl) {
		case LOG_CRITICAL:
			return "CRITICAL";
		case LOG_ERROR:
			return "ERROR";
		case LOG_WARN:
			return "WARN";
		case LOG_INFO:
			return "INFO";
		case LOG_DEBUG:
			return "DEBUG";
		case LOG_TRACE:
			return "TRACE";
		default:
			return "\0";
	}
}

uint8_t get_color_from_loglvl(int log_lvl) {
	switch (log_lvl) {
		case LOG_CRITICAL:
			return VGA_COLOR_MAGENTA;
		case LOG_ERROR:
			return VGA_COLOR_RED;
		case LOG_WARN:
			return VGA_COLOR_LIGHT_RED;
		case LOG_INFO:
			return VGA_COLOR_LIGHT_GREEN;
		case LOG_DEBUG:
			return VGA_COLOR_LIGHT_MAGENTA;
		case LOG_TRACE:
			return VGA_COLOR_LIGHT_CYAN;
		default:
			return VGA_COLOR_WHITE;
	}
}

void log_msg(int log_lvl, const char* unit, const char* msg) {
	char* pref = get_prefix_from_loglvl(log_lvl);
	if (pref[0] == '\0') {
		kernel_panic("Logging unit fail");
	}
	terminal_putchar('[');
	terminal_setcolor(get_color_from_loglvl(log_lvl));
	terminal_writestring(pref);
	terminal_setcolor(VGA_COLOR_WHITE);
	terminal_writestring("]{");
	terminal_writestring(unit);
	terminal_writestring("}:");
	terminal_writestring(msg);
	terminal_putchar('\n');
}


char* get_broadcast_prefix(int broadcast_status) {
	if (broadcast_status == BROADCAST_OK) {
		return "OK";
	} else { 
		return "FAIL";
	}
}
uint8_t get_color_from_broadcast_status(int status_code) {
	if (status_code == BROADCAST_OK) {
		return VGA_COLOR_GREEN;
	} else {
		return VGA_COLOR_LIGHT_RED;
	}
}

void broadcast_status(int broadcast_status_code, char* unit){ 
	terminal_putchar('[');
	terminal_setcolor(get_color_from_broadcast_status(broadcast_status_code));
	terminal_writestring(get_broadcast_prefix(broadcast_status_code));
	terminal_setcolor(VGA_COLOR_WHITE);
	terminal_writestring("]:");
	terminal_writestring(unit);
	terminal_putchar('\n');
}

void initialize_tty(void) {
	terminal_initialize();
	broadcast_status(BROADCAST_OK, "Terminal output");
}
