#include <stdnoreturn.h>

noreturn void main(void) {
	__asm__ volatile ("cli");
	while (1);
}
