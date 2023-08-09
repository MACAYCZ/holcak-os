[org 0x500]

[bits 16]
start_16:
	; Initialize registers
	mov bp, stack + stack_size
	mov sp, bp

	; Print debug message
	mov si, .debug
	call puts

	; Detect memory

	; TODO: Jump to stage3
	cli
	hlt

.debug:
	db "Debug: Entered stage2!", 0x0D, 0x0A, 0x00

%include "disk.inc"
%include "puts.inc"
%include "memory.inc"

stack_size: equ 0x100

section .bss
	stack: resb stack_size
section .text