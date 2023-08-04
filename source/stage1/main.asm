[org 0x7C00]

jmp short start_16

%if ($-$$) != 0x02
%error "Error: HFS1 header is expected to be on address 0x02!"
%endif

hfs1_magic: dd 0x00
hfs1_table: dd 0x00
hfs1_free:  dd 0x00

[bits 16]
start_16:
	; Initialize registers
	xor ax, ax
	mov ds, ax
	mov es, ax

	mov ss, ax
	mov sp, 0x7C00

	; Load first segment of the HFS1 table
	mov eax, [hfs1_table]
	mov ebx, (stage2_buffer << 0x0C) | (stage2_buffer % 0x10)
	mov cx,  0x01
	call disk_read

	; Load stage2 into memory
	mov esi, stage2_buffer
	mov eax, [esi+0x2C]
	mov cx,  [esi+0x30]
	call disk_read

	; Enter protected mode
	cli
	lgdt [start_gdt.desc]
	mov eax, cr0
	or eax, 0x01
	mov cr0, eax

	jmp code_seg:start_32

[bits 16]
%include "disk.inc"
%include "puts.inc"

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
	jmp stage2_buffer

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

stage2_buffer: equ 0x500

times 510-($-$$) db 0
dw 0xAA55
