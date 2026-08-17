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

void initialize_tty(void) {
	terminal_initialize();
}


