; Initializes disk
; Parameters:
;   dl    - Disk number
; References:
;   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=41h:_Check_Extensions_Present
;   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=08h:_Read_Drive_Parameters
;   https://stanislavs.org/helppc/int_13-8.html
disk_init:
	pusha
	mov ah, 0x41
	mov bx, 0x55AA
	int 0x13
	jc .continue
	mov [disk_extensions], cx

.continue:
	mov ah, 0x08
	int 0x13
	jc disk_error

	inc dh
	mov [disk_heads], dh
	mov bl, cl
	and bl, 0x3F
	mov [disk_sectors], bl
	shr cx, 0x06
	inc cx
	mov [disk_cylinders], cx

	popa
	ret

; Resets disk controller
; Parameters:
;   dl    - Disk number
disk_reset:
	pusha
	xor ah, ah
	int 13h
	jc disk_error
	popa
	ret

; Prints disk error and halts
; Parameters:
;   ah    - Status
disk_error:
	mov bl, ah
	mov bh, 0x13
	jmp pute

; Reads sectors from disk
; Parameters:
;   eax   - LBA pointing to the beginning of the data
;   ebx   - Address of buffer [offset 16] [segment 16]
;   si    - Number of sectors to read
;   dl    - Disk number
disk_read:
	push cx
	mov cx, [disk_extensions]
	test cx, 0x01
	pop cx
	jz disk_read_chs
	jmp disk_read_lba

; Reads sectors from disk
; Parameters:
;   eax   - LBA pointing to the beginning of the data
;   ebx   - Address of buffer [offset 16] [segment 16]
;   si    - Number of sectors to read
;   dl    - Disk number
; References:
;   https://stanislavs.org/helppc/int_13-2.html
disk_read_chs:
	pusha
	mov cx, es
	push cx

.loop:
	; TODO: I am overwriting eax
	; Initialize al (Number of sectors)
	mov al, 0x80
	cmp si, 0x80
	jge .buffer
	mov al, si

.buffer:
	; TODO: I am overwriting ebx
	; Initialize es:bx (Pointer to buffer)
	mov di, bx
	and di, 0x0F
	mov es, di
	shr ebx, 0x04

	; TODO: I am overwriting ebx
	; TODO: I am overwriting edx
	; Convert LBA to CHS
	xor edx, edx
	xor ebx, ebx
	mov bl, [disk_sectors]
	div ebx

	inc dl
	mov cl, dl

	xor edx
	xor edx, edx
	mov bl, [disk_heads]
	div ebx

	mov dh, dl
	mov ch, al
	shl ah, 0x06
	or cl, ah

	; Read sectors from disk
	mov di, 0x05

.retry:
	mov ah, 0x02
	push ax
	int 0x13
	jc .error
	pop ax
	; TODO: Add `Number of sectors read (al)` to eax, ebx and sub it from si
	jmp .loop

.error:
	test di, di
	jz disk_error
	call disk_reset
	dec di
	pop ax
	jmp .retry

.done:
	pop cx
	mov es, cx
	popa
	ret

section .bss
	disk_extensions: resb 0x01
	disk_cylinders:  resw 0x01
	disk_heads:      resb 0x01
	disk_sectors:    resb 0x01
section .text

; Reads sectors from disk
; Parameters:
;   %1    - (dword) LBA pointing to the beginning of the data
;   %2    - (dword) Address of buffer [offset 16] [segment 16]
;   %3    - (word)  Number of sectors to read
; References:
;   http://www.ctyme.com/intr/rb-0708.htm
;   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=42h:_Extended_Read_Sectors_From_Drive
%macro disk_packet_init 3
	mov byte  [disk_packet.size],     0x10
	mov byte  [disk_packet.reserved], 0x00
	mov word  [disk_packet.sectors],  %3
	mov dword [disk_packet.buffer],   %2
	mov dword [disk_packet.lba_low],  %1
	mov dword [disk_packet.lba_high], 0x00
%endmacro

; Reads sectors from disk
; Parameters:
;   eax   - LBA pointing to the beginning of the data
;   ebx   - Address of buffer [offset 16] [segment 16]
;   si    - Number of sectors to read
;   dl    - Disk number
; References:
;   http://www.ctyme.com/intr/rb-0708.htm
;   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=42h:_Extended_Read_Sectors_From_Drive
disk_read_lba:
	disk_packet_init eax, ebx, si

	push ax
	push si

	mov si, disk_packet
	mov ah, 0x42
	int 0x13
	jc .error

	pop si
	pop ax

	ret

.error:
	mov bl, ah
	mov bh, 0x13
	jmp pute

section .bss
disk_packet:
	.size:       resb 0x01
	.reserved:   resb 0x01
	.sectors:    resw 0x01
	.buffer:     resd 0x01
	.lba_low:    resd 0x01
	.lba_high:   resd 0x01
section .text
