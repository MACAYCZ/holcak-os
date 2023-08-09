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
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7C00
	call disk_init

	; Load first segment of the HFS1 table
	mov eax, [hfs1_table]
	mov ebx, (stage2_buffer << 0x0C) | (stage2_buffer % 0x10)
	mov si,  0x01
	call disk_read

	; Load stage2 into memory
	mov ecx, stage2_buffer
	mov eax, [ecx+0x2C]
	mov si,  [ecx+0x30]
	call disk_read

	; Jump to stage2
	jmp stage2_buffer

%include "disk.inc"
%include "puts.inc"

stage2_buffer: equ 0x500

times 510-($-$$) db 0
dw 0xAA55
