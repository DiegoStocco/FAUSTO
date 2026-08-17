.code16
.global _start


_start:
	cli
	# Saving boot drive number
	mov %dl, boot_drive

	# DEBUG TEXT 
	mov $'A', %al
	call putchar_realmode

	# Create stack
    	xorw %ax, %ax
    	movw %ax, %ds
    	movw %ax, %es
    	movw %ax, %ss
   	movw $0x7c00, %sp

    	sti

	# DEBUG
	mov $'B', %al
	call putchar_realmode

	call get_memory_map

	# DEBUG
	mov $'C', %al
	call putchar_realmode

	# Read kernel
	# Setting memory address for BIOS func call to read disk
	movw $0x1000, %ax
	movw %ax, %es
	xorw %bx, %bx	 # ES:BX = 0x1000:0000

	mov boot_drive, %dl # Disk number
	mov $0x02, %ah	 # BIOS read sector func
	mov $17, %al	 # Number of sector to read
	mov $0, %ch	 # Cylinder 0
	mov $2, %cl	 # Sector 2
	mov $0, %dh	 # Reading Head 0
	int $0x13

	# DEBUG
	jc disk_error
	mov $'R', %al
	call putchar_realmode

	# Load GDT
	cli 
	lgdt gdt_descriptor
	

	# Protected mode switch
	mov %cr0, %eax
	or $1, %al
	mov %eax, %cr0

	# Long jump
	ljmp $0x08, $protected_mode

disk_error:
	mov $'D', %al
	call putchar_realmode

	cli
	hlt
boot_drive:
	.byte 0
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

# -- DEBUG CHAR PRINT -- #
putchar_realmode:
	mov $0x0E, %ah
	mov $0x00, %bh
    	mov $0x07, %bl
    	int $0x10
	ret

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
	
	sti

	# jump to kernel addres
	jmp 0x10000
# Pad until 510 bytes and add MBR signature
.fill 510 - (. - _start), 1, 0
.word 0xAA55

