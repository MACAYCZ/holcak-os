; Initializes memory
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
memory_init:
	pushad
	push es

	; Initialize registers
	mov eax, 0xE820
	xor ebx, ebx
	mov ecx, memory_info_block
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
	pop es

	call memory_init_0xE820

	; Print debug information	
	mov si, .data_1
	call puts
	mov eax, [memory_info.length]
	call putx32
	call putel

	mov si, .data_2
	call puts
	mov eax, [memory_info.block]
	call putx32
	call putel

	popad
	ret

.failed:
	; TODO: Try different interrupt! https://wiki.osdev.org/Detecting_Memory_(x86)#Other_Methods
	mov si, .data_0
	call puts
	cli
	hlt

	pop es
	popad

.data_0: db "Error: Memory initialization failed!", 0x00
.data_1: db "Memory info length: 0x", 0x00
.data_2: db "Memory info block:  0x", 0x00

; Initializes memory using int 0x15, ah=0xE820
; References:
;   http://www.ctyme.com/intr/rb-1741.htm
memory_init_0xE820:
	pushad

	; Initialize registers
	xor ebx, ebx
	mov edx, 0x534D4150
	mov ecx, [memory_info.block]
	mov edi, memory_info.data
	xor esi, esi

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

	; Test if size of block is valid
	mov eax, [memory_info.block]
	cmp eax, ecx
	jne .error

	; Add offset to buffer and increment length
	pop edi
	call memory_block_print
	add edi, memory_info_block
	inc si

	test ebx, ebx
	jnz .loop

.done:
	mov [memory_info.length], si
	popad
	ret

.warning:
	push si
	mov si, .data_1
	call puts
	pop si
	jmp .done

.error:
	mov si, .data_0
	call puts
	cli
	hlt

.data_0: db "Error: Memory initialization (0xE820) failed!", 0x00
.data_1: db "Memory info length exceeded maximum!", 0x0D, 0x0A, 0x00

; Prints information about memory block
; Parameters:
;   edi   - Memory block address
memory_block_print:
	push eax
	push si

	mov si, .data_0
	call puts

	mov eax, [edi+0x04]
	call putx32
	mov eax, [edi+0x00]
	call putx32

	mov si, .data_1
	call puts

	mov eax, [edi+0x0C]
	call putx32
	mov eax, [edi+0x08]
	call putx32

	mov si, .data_2
	call puts

	mov eax, [edi+0x10]
	call putx32

	mov si, .data_3
	call puts

	pop si
	pop eax
	ret

.data_0: db "Memory detected: 0x", 0x00
.data_1: db " = 0x", 0x00
.data_2: db " (0x", 0x00
.data_3: db ")", 0x0D, 0x0A, 0x00

memory_info_length: equ 0x100
memory_info_block:  equ 0x18

section .bss
memory_info:
	.length: resd 0x01 ; Number of blocks
	.block:  resd 0x01 ; Size of block in bytes
	.data:   resb memory_info_length * memory_info_block
section .text
