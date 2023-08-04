%define ENDL 0x0D, 0x0A

; Prints string ending with zero byte
; Parameters:
;   ds:si - String address
puts:
	push ax
	push si
	mov ah, 0x0E

.loop:
	lodsb
	or al, al
	jz .done
	int 0x10
	jmp .loop

.done:
	pop si
	pop ax
	ret

; Prints hexadecimal value
; Parameters:
;   bx    - Value
puth:
	push ax
	push bx
	push cx

	mov ah, 0x0E
	mov cl, 0x03

	mov al, 0x30
	int 0x10
	mov al, 0x78
	int 0x10

.loop:
	mov al, bh
	shr al, 0x04
	cmp al, 0x0A
	jl .dump
	add al, 0x07

.dump:
	add al, 0x30
	int 0x10

	or cl, cl
	jz .done

	dec cl
	shl bx, 0x04
	jmp .loop

.done:
	pop cx
	pop bx
	pop ax
	ret

; Prints error code and halts
; Parameters:
;   bx    - Error code
pute:
	mov si, .data
	call puts
	call puth
	cli
	hlt

.data:
	db "Error: ", 0x0
