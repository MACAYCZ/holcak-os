; Initializes memory
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
	mov edi, memory_info.data
	shr edi, 0x04
	mov es, di
	mov edi, memory_info.data
	and di, 0x0F

	; Check if int 0x15, ah=0xE820 is supported
	int 0x15
	cmp eax, edx
	jne .failed

	; Save size of `memory_block`
	mov [memory_info.block], ecx
	mov si, .data_1
	call puts
	mov eax, ecx
	call putx32
	call putel

	pop es
	popad
	call memory_init_0xE820

	; Print memory blocks size	
	push ax
	push si
	mov si, .data_2
	call puts
	mov ax, [memory_info.length]
	call putx16
	call putel
	pop si
	pop ax
	ret

.failed:
	; TODO: Try different interrupt!
	mov si, .data_0
	call puts
	cli
	hlt

	pop es
	popad

.data_0: db "Error: memory_init failed!", 0x00
.data_1: db "Memory info block size: 0x", 0x00
.data_2: db "Memory info length: 0x", 0x00

;  Initializes memory using int 0x15, ah=0xE820
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
memory_init_0xE820:
	pushad

	; Initialize registers
	xor ebx, ebx
	mov edx, 0x534D4150
	mov edi, memory_info.data
	xor esi, esi
	mov ecx, [memory_info.block]

	; Test if size of `memory_block_size` is valid
	cmp ecx, 0x14
	je .loop
	cmp ecx, 0x18
	je .loop

	; Error: `memory_block_size` is invalid!
	mov si, .data_0
	call puts
	cli
	hlt

.loop:
	; Test if we exceeded maximum length
	cmp esi, memory_info_length
	jge .warning

	; Initialize es:di (Pointer to buffer)
	push edi
	shr edi, 0x04
	mov es, di
	mov edi, [esp+0x00]
	and di, 0x0F

	mov eax, 0xE820
	int 0x15
	jc .error

	; Test if size of `memory_block_size` is valid
	mov eax, [memory_info.block]
	cmp eax, ecx
	jne .error

	; Add offset to buffer and increment `memory_blocks_size`
	pop edi
	call memory_print_block
	add edi, ecx
	inc si

	test ebx, ebx
	jnz .loop

.done:
	mov [memory_info.length], si
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

.data_0: db "Error: Memory block size is invalid!", 0x00
.data_1: db "Error: memory_init_0xE820 failed!", 0x00
.data_2: db "Error: Memory blocks length exceeded maximum!", 0x0D, 0x0A, 0x00

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

memory_info_length: equ 0x100
memory_info_block:  equ 0x18

section .bss
memory_info:
	.length: resd 0x01 ; Number of blocks
	.block:  resd 0x01 ; Size of block in bytes
	.data:   resb memory_info_length * memory_info_block
section .text
