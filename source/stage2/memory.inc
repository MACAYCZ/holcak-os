; Initializes memory (fills the `memory_blocks`)
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
memory_init:
	pushad
	push es

	; Initialize registers
	mov eax, 0xE820
	xor ebx, ebx
	xor ecx, ecx
	dec ecx
	mov edx, 0x534D4150

	; Initialize es:di (Pointer to buffer)
	mov edi, memory_blocks
	shr edi, 0x04
	mov es, di
	mov edi, memory_blocks
	and di, 0x0F

	; Check if int 0x15, ah=0xE820 is supported
	int 0x15
	cmp eax, edx
	jne .failed

	; Save size of `memory_block`
	mov [memory_block_size], ecx
	mov si, .data_1
	call puts
	mov eax, ecx
	call putx32
	call putel

	pop es
	popad

	jmp memory_init_0xE820

.failed:
	; TODO: Try different interrupt!
	mov si, .data_0
	call puts
	cli
	hlt

	pop es
	popad

.data_0: db "Error: `memory_init` failed!", 0x00
.data_1: db "Memory block size: 0x", 0x00

; Fills the `memory_blocks` using int 0x15, ah=0xE820
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
memory_init_0xE820:
	pushad

	; Initialize registers
	xor ebx, ebx
	mov edx, 0x534D4150
	mov edi, memory_blocks
	xor si, si
	mov ecx, [memory_block_size]
	cmp ecx, 0x14
	je .loop
	cmp ecx, 0x18
	je .loop

	; Error: Unknown `memory_block_size`
	mov si, .data_0
	call puts
	cli
	hlt

.loop:
	cmp si, memory_blocks_max_size
	je .warning

	; Initialize es:di (Pointer to buffer)
	push edi
	shr edi, 0x04
	mov es, di
	mov edi, [esp+0x00]
	and di, 0x0F

	mov eax, 0xE820
	int 0x15
	jc .error

	; Test if `memory_block_size` is still same
	mov eax, [memory_block_size]
	cmp eax, ecx
	jne .error

	pop edi
	call memory_print_block
	add edi, ecx
	inc si

	test ebx, ebx
	jnz .loop

.done:
	mov [memory_blocks_size], si
	popad
	ret

.warning:
	push si
	mov si, .data_2
	call puts
	pop si
	jmp .done

.error:
	mov si, .data_1
	call puts
	cli
	hlt

.data_0: db "Error: Unknown memory block size!", 0x00
.data_1: db "Error: int 0x15, ah=0xE820 failed!", 0x00
.data_2: db "Error: Not enough space for memory_blocks!", 0x0D, 0x0A, 0x00

; Prints debug information about block
; Parameters:
;   edi   - Block address
memory_print_block:
	push si
	push eax

	mov si, .data_0
	call puts

	; Print base address
	mov eax, [edi+0x04]
	call putx32
	mov eax, [edi+0x00]
	call putx32

	mov si, .data_1
	call puts

	; Print length in bytes
	mov eax, [edi+0x0C]
	call putx32
	mov eax, [edi+0x08]
	call putx32

	mov si, .data_2
	call puts

	; Print type
	mov eax, [edi+0x10]
	call putx32

	mov si, .data_3
	call puts

	pop eax
	pop si
	ret

.data_0: db "Memory detected: 0x", 0x00
.data_1: db " = 0x", 0x00
.data_2: db " (0x", 0x00
.data_3: db ")", 0x0D, 0x0A, 0x00

memory_block_usable:       equ 0x00
memory_block_reserved:     equ 0x01
memory_block_acpi_reclaim: equ 0x02
memory_block_acpi_nvs:     equ 0x03
memory_block_invalid:      equ 0x04

memory_block_max_size:  equ 0x18
memory_blocks_max_size: equ 0x100

section .bss
	memory_block_size:  resd 0x01
	memory_blocks:      resb memory_blocks_max_size * memory_block_max_size
	memory_blocks_size: resw 0x01
section .text
