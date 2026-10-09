#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <kernel/log.h>
#include <kernel/memory.h>
#include <kernel/idt.h>
#include <kernel/keyboard.h>
#include <stdio.h>

// Checking for wrong OS target for the compiler // 
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This kernel must be compiled with an i386-elf compiler"
#endif

extern uint32_t _kernel_end; // Declared in linker script

void kernel_main(unsigned int magic, void* mb_info) {
	initialize_tty();
	testcolor_tty();
	log_msg(LOG_INFO,"kernel","FAUSTO kernel running Version 1.0.0");
	// - MEMORY - //
	init_memory();
	memory_checksum();
	// - IDT - //
	init_idt();

	log_msg(LOG_INFO, "system", "done.");
	
	while(1) {
		char c = getchar();
		char* s = (char*)&c;
		log_msg(LOG_DEBUG, "kernel",s);
	}
}
