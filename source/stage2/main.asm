[org 0x500]

[bits 16]
start_16:
	; Initialize registers
	mov bp, stack + stack_size
	mov sp, bp

	; Initialize memory
	call memory_init

	; TODO: Protected mode, load stage3
	cli
	hlt

%include "disk.inc"
%include "puts.inc"
%include "memory.inc"

stack_size:  equ 0x100
section .bss
	stack:  resb stack_size
section .text
