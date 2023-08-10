[bits 32]
[extern main]

section .entry
[global start]
start:
	cli
	mov ebp, stack + stack_size
	mov esp, ebp

	jmp main

stack_size: equ 0x2000
section .bss
stack:
	resd stack_size
section .text
