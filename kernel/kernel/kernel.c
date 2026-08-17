#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/log.h>
#include <kernel/memory.h>

// Checking for wrong OS target for the compiler // 
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This kernel must be compiled with an i386-elf compiler"
#endif

void kernel_main(void) {
	initialize_tty();
	log_msg(LOG_INFO,"kernel","FAUSTO kernel running Version 1.0.0");
	memory_checksum();
	log_msg(LOG_INFO, "system", "done.");
}
