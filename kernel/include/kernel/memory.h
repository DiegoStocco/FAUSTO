#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

unsigned int read_cr0(void);
int paging_enabled(void);
void memory_checksum(void);
void init_paging(void);
void enable_paging_asm(uint32_t page_directory);
void init_memory(void);
#endif
