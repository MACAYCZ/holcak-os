[bits 32]
; [extern main]

stack_size: equ 0x2000

section .entry
[global start]
start:
	cli
	mov ebp, stack + stack_size
	mov esp, ebp

	mov [0xB8000], byte 'B'
	cli
	hlt

section .bss
stack:
	resd stack_size
