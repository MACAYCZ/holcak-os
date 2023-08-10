[org 0x500]

[bits 16]
start_16:
	; Initialize registers
	mov bp, stack + stack_size
	mov sp, bp

	; Initialize memory
	call memory_init
	mov si, .data_0
	call puts
	mov ax, [memory_blocks_size]
	call putx16
	call putel

	; TODO: Protected mode, load stage3
	cli
	hlt

.data_0: db "Memory blocks received: 0x", 0x00

%include "disk.inc"
%include "puts.inc"
%include "memory.inc"

stack_size:  equ 0x100
blocks_size: equ 0x100

section .bss
	stack:  resb stack_size
	
section .text
