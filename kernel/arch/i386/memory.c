#include <kernel/memory.h>
#include <kernel/log.h>

unsigned int read_cr0(void) {
	unsigned int value;
	asm volatile ("mov %%cr0, %0" : "=r"(value));
	return value;
}

int paging_enabled(void) {
	unsigned int cr0 = read_cr0();
	if (cr0 & 0x80000000) {
		return 0;
	} else {
		return 1;
	}
}

void memory_checksum(void) {
	if(!paging_enabled()) {
		log_msg(LOG_CRITICAL, "Memory", "Paging is not enabled on the system");
		broadcast_status(BROADCAST_FAIL, "Paging");
		kernel_panic("Paging failed to start");			
	}

	// END
	broadcast_status(BROADCAST_OK, "Paging");
}

