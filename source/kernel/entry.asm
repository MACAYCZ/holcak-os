[bits 32]
[extern main]

stack_size: equ 0x2000

section .entry
[global start]
start:
	cli
	mov ebp, stack + stack_size
	mov esp, ebp
	sti

	jmp main

section .bss
stack:
	resd stack_size
