[org 0x500]

[bits 16]
start_16:
	; Initialize registers
	mov bp, stack + stack_size
	mov sp, bp
	call disk_init
	call memory_init

	; Load first segment of the HFS1 table
	mov eax, [hfs1_table]
	mov ebx, (stage3_buffer << 0x0C) | (stage3_buffer % 0x10)
	mov si,  0x01
	call disk_read

	; Load stage3 into memory
	mov ecx, stage3_buffer
	mov eax, [ecx+0x40]
	mov si,  [ecx+0x44]
	call disk_read

	; TODO: Enable A20
	; Enter protected mode
	lgdt [start_gdt.desc]
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax

	jmp code_seg:start_32

[bits 16]
%include "disk.inc"
%include "puts.inc"
%include "memory.inc"

[bits 32]
start_32:
	; Initialize registers
	mov ax, data_seg
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Jump to stage2 entry
	jmp stage3_buffer

start_gdt:
.null:
	dd 0x00
	dd 0x00
.code:
	dw 0xFFFF
	dw 0x00
	db 0x00
	db 0x9A
	db 0xCF
	db 0x00
.data:
	dw 0xFFFF
	dw 0x00
	db 0x00
	db 0x92
	db 0xCF
	db 0x00
.desc:
	dw .desc - start_gdt - 1
	dw start_gdt

code_seg: equ start_gdt.code - start_gdt
data_seg: equ start_gdt.data - start_gdt

hfs1_table:    equ 0x7C06
hfs1_free:     equ 0x7C0A
stage3_buffer: equ 0x20000

stack_size: equ 0x100
section .bss
	stack:  resb stack_size
section .text
