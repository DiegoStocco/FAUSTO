# -- ISR WRAPPER -- #

# Define macros for isr
.macro ISR_NO_ERR num
.type isr\num @function
isr\num:
	cli
	push $0             # Pushes dummy error code
	push $\num          # Pushes the interrupt numer
	jmp isr_common_stub
.endm


.macro ISR_ERR num
.type isr\num @function
isr\num:
	cli
	# Error code is pushed by the CPU
	push $\num          # Pushes the interrupt numer
	jmp isr_common_stub
.endm

# Table of isr
.global isr_table
isr_table:
	.irp i,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
	.long isr\i
	.endr

isr_common_stub:
	pusha		# Pushes all the registers to save them

	mov %ds, %ax
	push %eax	# Saved the data segment

	mov $0x10, %ax	# Loads the data sector of the kernel
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	push %esp
	call fault_handler # Calls C handler function
	add $4, %esp

	pop %eax	# Restores original segments
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	
	popa 		# Restores all the general registries
	add $8, %esp	# Cleans the stack from the initial pushes
	sti
	iret		# Return of the interrupt

ISR_NO_ERR 0
ISR_NO_ERR 1
ISR_NO_ERR 2
ISR_NO_ERR 3
ISR_NO_ERR 4
ISR_NO_ERR 5
ISR_NO_ERR 6
ISR_NO_ERR 7
ISR_ERR    8
ISR_NO_ERR 9
ISR_ERR    10
ISR_ERR    11
ISR_ERR    12
ISR_ERR    13
ISR_ERR    14
ISR_NO_ERR 15
ISR_NO_ERR 16
ISR_ERR    17
ISR_NO_ERR 18
ISR_NO_ERR 19
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_ERR    30
ISR_NO_ERR 31
