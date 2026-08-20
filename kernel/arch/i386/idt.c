#include <kernel/idt.h>

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

	// Implementation example for a specific interrupt (ex. IRS 0 for division by 0)
	// idt_set_gate(0, (uint32_t)isr0_wrapper, 0x08, 0x8E);
	
	idt_load();
}
