[bits 32]
[extern main]

section .entry
[global start]
start:
	cli
	mov ebp, stack + stack_size
	mov esp, ebp

	push ebx
	push eax
	call main

stack_size: equ 0x2000
section .bss
stack:
	resd stack_size
section .text
