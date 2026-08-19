#include <kernel/memory.h>
#include <kernel/log.h>
#include <stdint.h>
#include <stdio.h>

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));
extern uint32_t _kernel_end; // Declared in linker script

unsigned int read_cr0(void) {
	unsigned int value;
	asm volatile ("mov %%cr0, %0" : "=r"(value));
	return value;
}



int paging_enabled(void) {
	unsigned int cr0 = read_cr0();
	return (cr0 & 0x80000000) != 0;
}


// -- HEAP -- //
static block_header_t* heap_start = NULL;

void init_heap(uintptr_t start_addr, size_t heap_size) {
	// Initialize heap as a single huge free block
	heap_start = (block_header_t*)start_addr;
	heap_start->size = heap_size - sizeof(block_header_t);
	heap_start->is_free = 1;
	heap_start->next = NULL;
}

void* kmalloc(size_t size) {
	if (size == 0) return NULL;

	// Align 4 bytes for i386 arch
	size = (size + 3) & ~3;

	block_header_t* curr = heap_start;

	// Search first-fit
	while (curr) {
		if (curr->is_free && curr->size >= size) {
			// If the block is big enough, split it
			if (curr->size >= size + sizeof(block_header_t) + 4) {
				block_header_t* new_block = (block_header_t*)((uintptr_t)curr + sizeof(block_header_t) + size);
				new_block->size = curr->size - size - sizeof(block_header_t);
				new_block->is_free = 1;
				new_block->next = curr->next;

				curr->size = size;
				curr->next = new_block;
			
			}
			curr->is_free = 0;

			return (void*)((uintptr_t)curr + sizeof(block_header_t));
		}
		curr = curr->next;
	}
	return NULL; // Out of memory
}

void kfree(void* ptr) {
	if (!ptr) return;

	block_header_t* header = (block_header_t*)((uintptr_t)ptr - sizeof(block_header_t));

	// Defrag
	block_header_t* curr = heap_start;
	while (curr && curr->next) {
		if (curr->is_free && curr->next->is_free) {
			curr->size += sizeof(block_header_t) + curr->next->size;
			curr->next = curr->next->next;
		} else {
			curr = curr->next;
		}
	}
}

void init_memory(void) {
	uintptr_t heap_start = (uintptr_t)&_kernel_end;
	// Alignes at the healp start 4KB
	heap_start = (heap_start + 0xFFF) & ~0xFFF;

	// Esempio log di debug
	printf("Kernel End: 0x%x", (uintptr_t)&_kernel_end);
	printf("Heap Start: 0x%x\n", heap_start);

	init_heap(heap_start, 0x00100000);
}

void memory_checksum(void) {
	if(!paging_enabled()) {
		log_msg(LOG_ERROR, "Memory", "Paging is not enabled on the system");
		broadcast_status(BROADCAST_FAIL, "Paging");
		return;
	} else {
		broadcast_status(BROADCAST_OK, "Paging");
	}
	char* testbuf = (char*)kmalloc(64);
	if (testbuf){
		log_msg(LOG_INFO, "Memory", "Memory allocation test passed");
		broadcast_status(BROADCAST_OK, "Memory allocation");
		kfree(testbuf);
	} else {
		log_msg(LOG_CRITICAL, "Memory", "Memory allocation failed");
		broadcast_status(BROADCAST_FAIL, "Memory allocation");
		return;
	}
	

	// END

}
