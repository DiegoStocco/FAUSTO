/* Declare constanst for the multiboot header */
#include <stdlib.h>
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

.align 4096
page_directory:
	.skip 4096

.align 4096
page_table:
	.skip 4096

.section .text
.global _start
.type _start, @function
_start:
	/* 32-bit protected mode, no interrupts or paging active */
	cli
	/* setting up stack pointer */
	mov $stack_top, %esp

	# Save multiboot parameters in the C stack
	push %ebx	# Second arg (multiboot_info)
	push %eax	# Frist arg (0x2BADB02)

	# Paging config Identity mapping
	movl $page_table, %edi
	xorl %eax, %eax

1:
	movl %eax, (%edi)
	orl $0x003, (%edi)	# Present + writable
	addl $0x1000, %eax
	addl $4, %edi
	cmpl $0x400000, %eax
	jne 1b

	movl $page_table, %eax
	orl $0x003, %eax
	movl %eax, page_directory

	movl $page_directory, %eax
	movl %eax, %cr3

	movl %cr0, %eax
	orl $0x80000000, %eax
	movl %eax, %cr0
	
	# Enter the kernel
	call kernel_main

	# Infinite loop
	cli
2:	hlt
	jmp 2b

.size _start, . - _start
