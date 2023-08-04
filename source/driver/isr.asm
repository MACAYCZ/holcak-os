[extern isr_handler]

isr_common:
	pusha

	xor eax, eax
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	call isr_handler
	
	pop eax 
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8
	iret
	
%macro ISR_MAKE 1
[global isr_%1]
isr_%1:
%if !(%1 == 8 || (%1 >= 10 && %1 <= 14) || %1 == 17 || %1 == 21 || %1 == 29 || %1 == 30)
	push 0
%endif
	push %1
	jmp isr_common
%endmacro

%assign i 0
%rep 0xFF
	ISR_MAKE i
	%assign i i+1
%endrep
