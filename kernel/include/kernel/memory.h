#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

unsigned int read_cr0(void);
int paging_enabled(void);
void memory_checksum(void);
void init_memory(void);

// -- HEAP -- //
typedef struct block_header {
	size_t size;
	uint8_t is_free;
	struct block_header* next;
} block_header_t;

void init_heap(uintptr_t start_addr, size_t heap_size);
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif
