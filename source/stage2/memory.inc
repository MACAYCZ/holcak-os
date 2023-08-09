; References:
;   http://www.ctyme.com/intr/rb-1741.htm
memory_detect:
	

section .bss
struc memory_block
	.base_lo:   resd 0x00
	.base_hi:   resd 0x01
	.length_lo: resd 0x01
	.length_hi: resd 0x01
	.type:      resd 0x01
	.acpi:      resd 0x01
endstruc
section .text

memory_block_usable:       equ 0x00
memory_block_reserved:     equ 0x01
memory_block_acpi_reclaim: equ 0x02
memory_block_acpi_nvs:     equ 0x03
memory_block_invalid:      equ 0x04
