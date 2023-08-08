#include "printf.h"
#include <stdnoreturn.h>
#include <screen/vga.h>

noreturn void main(void) {
	vga_clear();
	vga_cursor_set(0);
	for (uint32_t i = 0; i < 24; i++) {
		printf("Iteration %u!\n", i);
	}
	__asm__ volatile ("cli");
	while (1);
}
