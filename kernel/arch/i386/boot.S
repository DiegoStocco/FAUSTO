/* Declare constanst for the multiboot header */
.set ALIGN, 	1<<0			/* align loaded modules on page boundaries */
.set MEMINFO, 	1<<1			/* provide memory map */
.set FLAGS,	ALIGN | MEMINFO		/* Multiboot 'flag' field */
.set MAGIC,	0x1BADB002		/* Magic number for bootloader */
.set CHECKSUM,	-(MAGIC + FLAGS)	/* checksum of above, to prove it's for multiboot */

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Small 16 KiB stack inizialization */

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
	/* 32-bit protected mode, no interrupts or paging active */

	/* Setting up the stack by assigning the 'esp' register the address of the top of the stack. esp will be the stack pointer */
	mov $stack_top, %esp

	/*
	This is a good place to initialize crucial processor state before the
	high-level kernel is entered. It's best to minimize the early
	environment where crucial features are offline. Note that the
	processor is not fully initialized yet: Features such as floating
	point instructions and instruction set extensions are not initialized
	yet. The GDT should be loaded here. Paging should be enabled here.
	C++ features such as global constructors and exceptions will require
	runtime support to work as well.
	*/

	/* Entering the high-level kernel. The ABI requires the stack to be 160byte aligned. */
	call kernel_main

	/* Infinite loop */
	cli
1:	hlt
	jmp 1b

/* Setting size of the _start symbol to the current location '.' minus it's start. */
.size _start, . - _start
