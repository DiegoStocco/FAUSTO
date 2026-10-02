#include <stdint.h>
#include "isr.h"
#include "keyboard.h"
#include "io.h"
#include <kernel/log.h>

// Error messages for the first 32 CPU exceptions
const char* exception_messages[32] = {
	"Division By Zero",
	"Debug",
	"Not Maskerable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack Segment Fault",
	"General Protection",
	"Page Fault",
	"[Reserved]",
	"Floating Point Error",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
	"Control Protection Exception",
	"[Reserved]",
	"[Reserved]",
	"[Reserved]",
	"[Reserved]",
	"[Reserved]",
	"[Reserved]",
	"[Reserved]",
	"[Reserved]",
	"[Reserved]",
	"[Reserved]",
};

void fault_handler(registers_t *regs) {
	if (regs->int_no < 32) {
		// CPU exceptions handling (0 - 31)
		log_msg(LOG_ERROR, "Interrupt Handler", exception_messages[regs->int_no]);
		while (1) {
			__asm__ volatile ("cli; hlt");
		}
	}
}

void keyboard_handler(void) {
	uint8_t scancode = inb(0x60);

	// Use scancode
	handle_scancode(scancode);	

	pic_send_eoi(1);
}
