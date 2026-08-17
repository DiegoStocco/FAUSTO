ENTRY(_start)

SECTIONS
{
	/* BIOS loads MBR at addr 0x7C00
	. = 0x7C00;

	.text :
	{
		*(.text)
	}

	.rodata :
	{
		*(.rodata)
	}

	.data :
	{
		*(.data)
	}

	.bss :
	{
		*(.bss)
	}

	/* discard compiler stuff */
	/DISCARD/ :
	{
		*(.comment)
		*(.note*)
		*(.eh_frame*)
	}
}
