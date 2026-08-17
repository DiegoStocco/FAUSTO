#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

unsigned int read_cr0(void);
int paging_enabled(void);
void memory_checksum(void);

#endif
