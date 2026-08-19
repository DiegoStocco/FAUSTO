ENTRY(_start)

SECTIONS
{
	/* 64kb Offset */
	. = 0x10000;

	/* Multiboot header */
	.text ALIGN(4K) :
	{
		*(.multiboot)
		*(.text)
	}
	
	/* Read-only data */
	.rodata ALIGN(4K) :
	{
		*(.rodata)	
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) :
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) :
	{
		*(COMMON)
		*(.bss)
	}

	_kernel_end = .;
}
