#include <stdint.h>
#include "pic.h"
#include <kernel/log.h>

static inline void outb(uint16_t port, uint8_t val) {
	__asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	__asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
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

	log_msg(LOG_INFO, "PIC", "PIC remapped");

	// Masks all the intterupts for now (1 = disabled)
	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);

	log_msg(LOG_INFO, "PIC", "All intterrupt masked");

	broadcast_status(BROADCAST_OK, "PIC");
}

void pic_clear_mask(uint8_t irq) {
	uint16_t port;
	if (irq < 8) {
		port = 0x21;
	} else {
		port = 0xA1;
		irq -= 8;
	}

	uint8_t value = inb(port) & ~(1 << irq);
	outb(port, value);
	log_msg(LOG_DEBUG, "PIC", "Interrupt IRQ? unmasked");
}
