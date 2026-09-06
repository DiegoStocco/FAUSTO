#include "idt_internal.h"
#include "isr.h"
#include <kernel/idt.h>
#include <kernel/log.h>

struct idt_entry idt[256];
struct idt_ptr idtp;

// populating IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
	idt[num].base_low = (base & 0xFFFF);
	idt[num].base_high = (base >> 16) & 0xFFFF;
	idt[num].sel = sel;
	idt[num].always0 = 0;
	idt[num].flags = flags; // 0x8E: Present, Ring 0, 32-bit Interrupt Gate
}

void init_idt(void) {
	idtp.limit = (sizeof(struct idt_entry)*256) -1;
	idtp.base = (uint32_t)&idt;

	// Reset IDT
	for (int i = 0; i < 256; i++) {
		idt_set_gate(i, 0, 0, 0);
	}

	// Set IDT for CPU faults
	for(int i = 0; i < 32; i++) {
		idt_set_gate(i, (uint32_t)isr_table[i], 0x08, 0x8E);
	}
		
	// EXTERNAL asm
	idt_load();
	__asm__ inline ("sti");
	log_msg(LOG_INFO, "IDT", "Interrupts enabled");
	broadcast_status(BROADCAST_OK, "IDT Setup");
}
