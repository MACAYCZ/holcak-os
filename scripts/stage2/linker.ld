ENTRY(start)
OUTPUT_FORMAT(binary)

SECTIONS
{
	. = 0x500;

	.entry  : {*(.entry) }
	.text   : {*(.text)  }
	.data   : {*(.data)  }
	.rodata : {*(.rodata)}
	.bss    : {*(.bss)   }
}
