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
	test al, al
	jz .done
	int 0x10
	jmp .loop

.done:
	pop si
	pop ax
	ret

; Prints 16 bit hexadecimal value (use putx16)
; Parameters:
;   bx    - Value
put16:
	; TODO: Add loop instruction
	push ax
	push bx
	push cx

	mov ah, 0x0E
	mov cx, 0x03

.loop:
	mov al, bh
	shr al, 0x04
	cmp al, 0x0A
	jl .dump
	add al, 0x07

.dump:
	add al, 0x30
	int 0x10

	test cx, cx
	jz .done

	dec cx
	shl bx, 0x04
	jmp .loop

.done:
	pop cx
	pop bx
	pop ax
	ret

; Prints 16 bit hexadecimal value starting with 0x
; Parameters:
;   bx    - Value
putx16:
	push ax
	mov ah, 0x0E
	mov al, 0x30
	int 0x10
	mov al, 0x78
	int 0x10
	call put16
	pop ax
	ret

; Prints 32 bit hexadecimal value starting with 0x
; Parameters:
;   ebx   - Value
putx32:
	push ebx
	shr ebx, 0x10
	call putx16
	pop ebx
	call put16
	ret

; Prints error code and halts
; Parameters:
;   bx    - Error code
pute:
	mov si, .data
	call puts
	call putx16
	cli
	hlt

.data:
	db "Error: ", 0x0
