[bits 32]
[extern main]

stack_size: equ 0x2000

section .entry
[global start]
start:
	mov ebp, stack + stack_size
	mov esp, ebp

	jmp main

section .bss
stack:
	resd stack_size
