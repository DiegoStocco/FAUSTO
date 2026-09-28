#include <stdint.h>
#include "io.h"

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC_EOI 0x20
void pic_send_eoi(uint8_t irq) {
	if (irq >= 8) {
		outb(PIC2_COMMAND, PIC_EOI);
	}
	outb(PIC1_COMMAND, PIC_EOI);
}



