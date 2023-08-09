; Initializes memory (fills the `memory_blocks`)
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
memory_init:
	; TODO: https://youtu.be/xp-yB9WBadI?t=1177
	jmp memory_init_0xE820

; Fills the `memory_blocks` using int 0x15, ah=0xE820
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
memory_init_block_0xE820:
	pusha
	push es

	; Initialize registers
	xor ebx, ebx
	mov ecx, 0x18
	mov edx, 0x534D4150
	mov edi, memory_blocks
	xor si, si

.loop:
	; Test if we have enough memory
	cmp si, memory_blocks_size
	; TODO: Not enough memory
	jge .error

	; Initialize es:di (Pointer to buffer)
	push edi
	mov ax, di
	and ax, 0x0F
	shr edi, 0x10
	mov es, di
	mov di, ax

	mov eax, 0x0000E820
	int 0x15
	jc .error

	cmp ecx, 0x18
	; TODO: Doesn't support ACPI
	jne .error

	; Restore the address
	pop edi
	
	; Print debug information
	push si
	mov si, .data_0
	call puts

	mov eax, [edi+memory_block.base_lo]
	call putx32
	mov eax, [edi+memory_block.base_hi]
	call putx32

	mov si, .data_1
	call puts

	mov eax, [edi+memory_block.length_lo]
	call putx32
	mov eax, [edi+memory_block.length_hi]
	call putx32

	mov si, .data_2
	call puts

	mov eax, [edi+memory_block.type]
	call putx32

	mov si, .data_3
	call puts
	pop si

	; Increment `memory_blocks_length`
	add edi, ecx
	inc si

	test ebx, ebx
	jnz .loop

.done:
	mov [memory_blocks_length], si
	pop es
	popa
	ret

.error:
	mov ax, 0xE820
	mov bl, 0x15
	call pute
	cli
	hlt

.data_0: db "Memory detected: 0x", 0x00
.data_1: db " = 0x", 0x00
.data_2: db " (0x", 0x00
.data_3: db ")", 0x0D, 0x0A, 0x00

memory_print_block:
	; TODO

memory_block_usable:       equ 0x00
memory_block_reserved:     equ 0x01
memory_block_acpi_reclaim: equ 0x02
memory_block_acpi_nvs:     equ 0x03
memory_block_invalid:      equ 0x04

memory_blocks_size:        equ 0x100

section .bss
struc memory_block
	.base_lo:   resd 0x01
	.base_hi:   resd 0x01
	.length_lo: resd 0x01
	.length_hi: resd 0x01
	.type:      resd 0x01
	.acpi:      resd 0x01
endstruc
memory_blocks:        resb memory_blocks_size * memory_block_size
memory_blocks_length: resw 0x01
section .text
