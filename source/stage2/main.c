#include "printf.h"
#include <stdnoreturn.h>
#include <screen/vga.h>

noreturn void main(void) {
	vga_clear();
	__asm__ volatile ("cli");
	while (1);
}
