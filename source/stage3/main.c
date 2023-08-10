#include <private/define.h>
#include <stdnoreturn.h>
#include <screen/vga.h>

noreturn _cdecl void main(void) {
	vga_clear();
	__asm__ volatile ("cli");
	while (1);
}
