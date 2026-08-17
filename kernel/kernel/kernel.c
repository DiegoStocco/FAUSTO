#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/tty.h>

// Checking for wrong OS target for the compiler // 
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This kernel must be compiled with an i386-elf compiler"
#endif

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

void kernel_main(void) {
	terminal_initialize();

	// placeholder message //
	terminal_writestring("FAUSTO kernel running\nVersion 1.0.0");

	kernel_panic("Test");
}
