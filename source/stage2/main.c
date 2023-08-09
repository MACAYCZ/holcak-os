#include "printf.h"
#include <stdnoreturn.h>
#include <screen/vga.h>

/*
TODO: Make printf something like this (printf will take function pointer as argument that will output character)
https://github.com/brainboxdotcc/retro-rocket/blob/d8d19bd52a809ffed2705aed60380a84cfe0ec84/src/printf.c#L18
*/

noreturn void main(void) {
	vga_clear();
	vga_cursor_set(0);
	printf("Hello, %d!\n", 0x1234);
	__asm__ volatile ("cli");
	while (1);
}
