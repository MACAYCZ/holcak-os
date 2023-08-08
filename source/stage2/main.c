#include <stdnoreturn.h>
#include <screen/vga.h>
#include <wchar.h>

noreturn void main(void) {
	__asm__ volatile ("cli");
	while (1);
}
