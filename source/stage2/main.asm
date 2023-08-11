[org 0x500]

[bits 16]
start_16:
	; Initialize registers
	mov bp, stack + stack_size
	mov sp, bp
	call disk_init
	call a20_enable
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

; Enables A20 line
; References:
;   https://wiki.osdev.org/A20_Line
a20_enable:
	push ax
	call a20_test
	jc .done

.try_0:
	mov ax, 0x2403
	int 0x15
	call a20_test
	jc .done

.try_1:
	xor al, al
	mov ah, 0x20
	in al, 0x92
	or al, 2
	out 0x92, al
	call a20_test
	jc .done
	test ah, ah
	dec ah
	jnz .try_1

.try_2:
	in al, 0xEE
	call a20_test
	jc .done

.error:
	mov si, .data_0
	call puts
	cli
	hlt

.done:
	pop ax
	clc
	ret

.data_0: db "Error: Enabling A20 line failed!", 0x00

; Tests if A20 line is enabled
; Returns:
;   cf    - 1 if enabled
; References:
;   https://wiki.osdev.org/A20_Line
a20_test:
	push ax
	push bx
	push ds
	push es
	clc

	; Initialize registers
	xor ax, ax
	mov ds, ax
	mov si, 0x500

	not ax
	mov es, ax
	mov di, 0x0510

	; Save old values
	mov bl, [ds:si]
	mov bh, [es:di]

	; Test if enabled
	mov ah, 0x01
	mov [ds:si], byte 0x00
	mov [es:di], byte 0x01
	mov al, [ds:si]
	cmp al, [es:di]
	je .done
	stc

.done:
	; Restore values
	mov [ds:si], bl
	mov [es:di], bh

	pop es
	pop ds
	pop bx
	pop ax
	ret

[bits 32]
start_32:
	; Initialize registers
	mov ax, data_seg
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Give parameters to stage3
	mov eax, disk_info
	mov ebx, memory_info

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
