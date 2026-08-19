#include <kernel/memory.h>
#include <kernel/log.h>
#include <stdint.h>

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));


unsigned int read_cr0(void) {
	unsigned int value;
	asm volatile ("mov %%cr0, %0" : "=r"(value));
	return value;
}
void init_paging(void) {
	// Initializing the page table with identity mapping (0 - 4MB)
	for (int i = 0; i < 1024; i++) {
		// Bit 0: Present (1), Bit 1: Read/Write (1), Bit 2: User/Supervisor (0 = kernel)
		first_page_table[i] = (i* 0x1000) | 3;
	}
	// insert the page table in the page directory
	page_directory[0] = ((uint32_t)first_page_table) | 3;

	// sets the other page directory entries as not present
	for (int i = 1; i < 1024; i++) {
		page_directory[i] = 0x00000002; // Not present, Read/Write
	}

	enable_paging_asm((uint32_t)page_directory);
}

void enable_paging_asm(uint32_t page_directory_ptr) {
	asm volatile (
			"mov %0, %%cr3\n\t"		// Loads the address of the page dir in CR3
			"mov %%cr0, %%eax \n\t" 	// Reads register CR0
			"or $0x80000000, %%eax\n\t"	// Sets the 31st bit (PG - Paging enable)
			"mov %%eax, %%cr0"		// Writes the updated value in CR0
			:
			: "r"(page_directory_ptr)
			: "eax", "memory"
	);
}


int paging_enabled(void) {
	unsigned int cr0 = read_cr0();
	return (cr0 & 0x80000000) != 0;
}



void memory_checksum(void) {
	if(!paging_enabled()) {
		log_msg(LOG_ERROR, "Memory", "Paging is not enabled on the system");
		broadcast_status(BROADCAST_FAIL, "Paging");
		return;
	}

	// END
	broadcast_status(BROADCAST_OK, "Paging");
}

void init_memory(void) {
	init_paging();
}
