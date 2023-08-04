; Reads sectors from disk
; Parameters:
;   %1    - (dword) LBA pointing to the beginning of the data
;   %2    - (dword) Address of buffer [offset 16], [segment 16]
;   %3    - (word)  Number of sectors to read
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
;   ebx   - Address of buffer [offset 16], [segment 16]
;   cx    - Number of sectors to read
;   dl    - Disk number
; References:
;   http://www.ctyme.com/intr/rb-0708.htm
disk_read:
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
disk_packet:
	.size:     resb 0x01
	.reserved: resb 0x01
	.sectors:  resw 0x01
	.buffer:   resd 0x01
	.lba_low:  resd 0x01
	.lba_high: resd 0x01
section .text
