#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
	__asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void pic_remap(void) {
	// Initialization start of the PIC master and slave (cascade mode)
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	// Remapping vectors offset: Master -> 32 (0x20), slave -> 40 (0x28)
	outb(0x21, 0x20);
	outb(0xA1, 0x28);

	// Cascading configuration
	outb(0x21, 0x04);
	outb(0xA1, 0x02);

	// 8086/88 mode
	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	// Masks all the intterupts for now (1 = disables)
	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);
}
