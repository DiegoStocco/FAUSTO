.code16
.global _start

# -- MAIN BOOTLOADER SECTION -- #

_start:
	cli

	# Saving boot drive number
	mov %dl, boot_drive

	# Create stack
	xorw %ax, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $0x7c00, %sp

	call get_memory_map

	# Read kernel
	call setup_read_call
	call read_kernel

	# Load GDT
	lgdt gdt_descriptor
	
	# Protected mode switch
	call switch_protected_mode

	# Long jump into protected mode
	ljmp $0x08, $protected_mode

# -- 16-bit FUNCTIONS DEFINITIONS -- #

# - Function: setup_read_call - #
setup_read_call:
	# Setting memory address for BIOS func call to read disk
	movw $0x1000, %ax
	movw %ax, %es
	xorw %bx, %bx	 # ES:BX = 0x1000:0000
	ret
	
# - Function: read_kernel - #
read_kernel:
	mov boot_drive, %dl # Disk number
	mov $0x02, %ah	 # BIOS read sector func
	mov $27, %al	 # Number of sector to read
	mov $0, %ch	 # Cylinder 0
	mov $2, %cl	 # Sector 2
	mov $0, %dh	 # Reading Head 0
	int $0x13
	jc throw_disk_error
	ret

# - Function: switch_protected_mode - #
switch_protected_mode:
	mov %cr0, %eax
	or $1, %al
	mov %eax, %cr0
	ret

# - Function: putchar_realmode - #
putchar_realmode:
	push %ax
	push %bx
	
	mov $0x0E, %ah
	mov $0x00, %bh
    	mov $0x07, %bl
    	int $0x10

	pop %bx
	pop %ax
	ret

# - Function: print_string - #
print_string:
	push %ax
	push %si

.print_loop:
	mov (%si), %al
	test %al, %al
	jz .print_done

	call putchar_realmode
	inc %si
	jmp .print_loop

.print_done:
	pop %si
	pop %ax
	ret

# -- ERROR FUNCTIONS -- #
disk_error_msg:
	.asciz "Disk read failed.\0"

throw_disk_error:
	mov $disk_error_msg, %si	
	call print_string
	
	cli
	hlt

dap:
	.byte 16
	.byte 0
	.word 24
	.word 0x0000
	.word 0x1000
	.long 1
	.long 0

# -- PHYSICAL MEMORY MAP -- # 
get_memory_map:
	mov $0x9000, %di	# Dest addr for mmap record
	xor %ebx, %ebx		# Continuation  value

read_e820_loop:
	mov $0xE820, %eax
	movl $0x534D4150, %edx	# 'SMAP'
	mov $24, %ecx		# Entry buffer size
	int $0x15
	jc e820_done		# Carry Flag set

	add $24, %di		# Next slot
	test %ebx, %ebx
	jnz read_e820_loop	# If EBX != 0 there are more blocks

e820_done:
	# Populate structure Multiboot_info at 0x9500
	movl $0x40, 0x9500	# flags: bit 6 (mmap_*) valid
	movl $0x9000, (0x9548)	# mmap_addr
	# Map len
	sub $0x9000, %di
	movzx %di, %eax
	movl %eax, (0x9544)	# mmap_length
	ret

# -- GLOBAL DESCRIPTOR TABLE -- #
.code16
gdt_start:
	.quad 0x0			# Null descriptor
	.quad 0x00cf9a000000ffff	# 32-bit code segment
	.quad 0x00cf92000000ffff	# 32-bit data segment
gdt_end:

gdt_descriptor:
	.word gdt_end - gdt_start - 1
	.long gdt_start

boot_drive:
	.byte 0

# -- CODE32 PROTECTED MODE -- #

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

	# Multiboot1 signature (response magic)
	mov $0x2BADB002, %eax

	# jump to kernel addres
	jmp 0x10000

# Pad until 510 bytes and add MBR signature
.fill 510 - (. - _start), 1, 0
.word 0xAA55

