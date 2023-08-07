ENTRY(start)

SECTIONS
{
	. = 0x20000;

	.entry  : {*(.entry) }
	.text   : {*(.text)  }
	.data   : {*(.data)  }
	.rodata : {*(.rodata)}
	.bss    : {*(.bss)   }
}
