#include <stdint.h>
#include <kernel/isr.h>

// Error messages for the first 32 CPU exceptions
const char* exception_messages[] = {
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
};

void fault_handler(registers_t *regs) {
	if (regs->int_no < 32) {
		// CPU exceptions handling (0 - 31)
		

		while (1) {
			__asm__ volatile ("cli; hlt");
		}
	}
}
