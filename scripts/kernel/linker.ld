ENTRY(start)

SECTIONS
{
	. = 0x100000;

	.entry  : {*(.entry) }
	.text   : {*(.text)  }
	.data   : {*(.data)  }
	.rodata : {*(.rodata)}
	.bss    : {*(.bss)   }
}
