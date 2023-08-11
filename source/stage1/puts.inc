%define endl 0x0D, 0x0A

; Prints string ending with NUL
; Parameters:
;   ds:si - String address
; References:
;   https://stanislavs.org/helppc/int_10-e.html
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

; Prints end line
; References:
;   https://stanislavs.org/helppc/int_10-e.html
putel:
	push si
	mov si, .data
	call puts
	pop si
	ret

.data:
	db endl, 0x00

; Prints 4 bit hexadecimal value (value bigger than 0x0F results in UB)
; Parameters:
;   al    - Value
; References:
;   https://stanislavs.org/helppc/int_10-e.html
putx4:
	push ax
	mov ah, 0x0E

	cmp al, 0x0A
	jl .dump
	add al, 0x07

.dump:
	add al, 0x30
	int 0x10

	pop ax
	ret

; Prints 8 bit hexadecimal value
; Parameters:
;   al    - Value
putx8:
	push ax
	shr al, 0x04
	call putx4
	mov al, [esp]
	and al, 0x0F
	call putx4
	pop ax
	ret

; Prints 16 bit hexadecimal value
; Parameters:
;   ax    - Value
putx16:
	push ax
	mov al, ah
	call putx8
	pop ax
	call putx8
	ret

; Prints 32 bit hexadecimal value
; Parameters:
;   eax   - Value
putx32:
	push eax
	shr eax, 0x10
	call putx16
	pop eax
	call putx16
	ret

; Prints generic error message
; Parameters:
;   ax    - Error code
;   bl    - Interrupt
pute:
	mov si, .data_0
	call puts

	push ax
	mov al, bl
	call putx8
	pop ax

	call putx16
	cli
	hlt

.data_0: db "Error: 0x", 0x0
