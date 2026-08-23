# -- ISR WRAPPER -- #

.global isr0
.type isr0, @function

isr0: 
	cli 
	push $0		# Pushes dummy error code
	push $0		# Pushes the interrupt numer (0)
	jmp isr_common_stub

isr_common_stub:
	pusha		# Pushes all the registers to save them

	mov %ds, %ax
	push %eax	# Saved the data segment

	mov $0x10, %ax	# Loads the data sector of the kernel
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	call fault_handler # Calls C handler function

	pop %eax	# Restores original segments
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	
	popa 		# Restores all the general registries
	add $8, %esp	# Cleans the stack from the initial pushes
	sti
	iret		# Return of the interrupt


