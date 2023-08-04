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
	jc .error

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

.error:
	mov bl, ah
	mov bh, 0x13
	jmp pute

; Reads sectors from disk
; Parameters:
;   eax   - LBA pointing to the beginning of the data
;   ebx   - Address of buffer [offset 16] [segment 16]
;   cx    - Number of sectors to read
;   dl    - Disk number
disk_read:
	; TODO: Maybe write the lba and chs read directly to this function?
	push di
	mov di, [disk_extensions]
	test di, 0x01
	pop di
	jz .chs

.lba:
	call disk_read_lba
	ret

.chs:
	call disk_read_chs
	ret

; Reads sectors from disk
; Parameters:
;   eax   - LBA pointing to the beginning of the data
;   ebx   - Address of buffer [offset 16] [segment 16]
;   cx    - Number of sectors to read
;   dl    - Disk number
; References:
;   https://stanislavs.org/helppc/int_13-2.html
disk_read_chs:

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
;   cx    - Number of sectors to read
;   dl    - Disk number
; References:
;   http://www.ctyme.com/intr/rb-0708.htm
;   https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=42h:_Extended_Read_Sectors_From_Drive
disk_read_lba:
	disk_packet_init eax, ebx, cx

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
disk_extensions: resb 0x01
disk_cylinders:  resw 0x01
disk_heads:      resb 0x01
disk_sectors:    resb 0x01

disk_packet:
	.size:       resb 0x01
	.reserved:   resb 0x01
	.sectors:    resw 0x01
	.buffer:     resd 0x01
	.lba_low:    resd 0x01
	.lba_high:   resd 0x01
section .text
