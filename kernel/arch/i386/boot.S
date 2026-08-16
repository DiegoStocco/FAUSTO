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


.section .data

.align 8
gdt:
	.quad 0x0000000000000000	# 0x00: null descriptor

	# 0x08: code segment
	.word 0xFFFF			# limit low
	.word 0x0000			# base low
	.byte 0x00			# base middle
	.byte 0x9A			# access: present, ring 0, code ,readable
	.byte 0xCF			# granularity=4K, 32-but, limit high
	.byte 0x00			# base high

	# 0x10: data segment
	.word 0xFFFF
	.word 0x0000
	.byte 0x00
	.byte 0x92			# present, ring 0, data, writable
	.byte 0xCF			# granularity=4K, 32-bit
	.byte 0x00
gdt_end:

gdt_descriptor:
	.word gdt_end - gdt - 1		# GDT limit
	.long gdt			# GDT base

.section .text
.global _start
.type _start, @function
_start:
	/* 32-bit protected mode, no interrupts or paging active */
	cli
	/* setting up stack pointer */
	mov $stack_top, %esp

	/* Load GDT */
	lgdt gdt_descriptor

	/* Reload CS */
	ljmp $0x08, $protected_mode
protected_mode:
	/* initializing segment register */
	mov $0x10, %ax
	

	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss


	/* Setting up paging */
	movl $page_table, %edi
	xorl %eax, %eax

	1:
		movl %eax, (%edi)
		orl $0x003, (%edi)	# present + writable
	
		addl $0x1000, %eax
		addl $4, %edi

		cmpl $0x400000, %eax
		jne 1b

	# page_directory[0] = page_table | PRESENT | WRITEABLE
	movl $page_table, %eax
	orl  $0x003, %eax
	movl %eax, page_directory

	# CR3 = page directory
	movl $page_directory, %eax
	movl %eax, %cr3

	# CR0.PG = 1
	movl %cr0, %eax
	orl  $0x80000000, %eax
	movl %eax, %cr0	


	/* Stack is already set */
	/* Entering the high-level kernel. The ABI requires the stack to be 16 byte aligned. */
	call kernel_main

	/* Infinite loop */
	cli
2:	hlt
	jmp 2b

/* Setting size of the _start symbol to the current location '.' minus it's start. */
.size _start, . - _start
