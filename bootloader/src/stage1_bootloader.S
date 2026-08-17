.code16
.global _start


_start:
	# Segment reset
	cli
	xorw %ax, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $0x7c00, %sp
	sti

	call get_memory_map

	# Read kernel
	movw $0x1000, %ax
	movw %ax, %es
	xorw %bx, %bx	 # ES:BX = 0x1000:0000

	mov $0x02, %ah	 # BIOS read sector func
	mov $32, %al	 # Number of sector to read
	mov $0, %ch	 # Cylinder 0
	mov $2, %cl	 # Sector 2
	mov $0, %dh	 # Head 0
	int $0x13


	# Load GDT
	cli 
	lgdt gdt_descriptor

	# Protected mode switch
	mov %cr0, %eax
	or $1, %eax
	mov %eax, %cr0

	# Long jump
	ljmp $0x08, $protected_mode

get_memory_map:
	mov $0x9000, %di	# Dest addr for mmap record
	xor %ebx, %ebx		# Continuation  value

read_e820_loop:
	mov $0xE820, %eax
	movl $0x0534D4150, %edx	# 'SMAP'
	mov $24, %ecx		# Entry buffer size
	int $0x15
	jc e820_done		# Carry Flag set

	add $24, %di		# Next slot
	test %ebx, %ebx
	jnz read_e820_loop		# If EBX != 0 there are more blocks

e820_done:
	# Populate structure Multiboot_info at 0x9500
	movl $0x40, 0x9500	# flags: bit 6 (mmap_*) valid
	movl $0x9000, (0x9548)	# mmap_addr
	# Map len
	sub $0x9000, %di
	movzx %di, %eax
	movl %eax, (0x9544)	# mmap_length
	ret

.code32
protected_mode:
	# Setting 32-bit data registries  
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs 
	mov %ax, %gs 
	mov %ax, %ss 

	# EBX pointing to multiboot_info in RAM
	mov $0x9500, %ebx

	# Multiboot1 signature
	mov $0x2BADB002, %eax

	# jump to kernel addres
	jmp 0x10000

# GDT
gdt_start:
	.quad 0x0			# Null descriptor
	.quad 0x00cf9a000000ffff	# 32-bit code segment
	.quad 0x00cf92000000ffff	# 32-bit data segment
gdt_end:

gdt_descriptor:
	.word gdt_end - gdt_start -1
	.long gdt_start

# Pad until 510 bytes and add MBR signature
.fill 510 - (. - _start), 1, 0
.word 0xAA55

