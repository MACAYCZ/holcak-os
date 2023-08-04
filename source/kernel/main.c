#include <isr.h>
#include <pic.h>
#include <pit.h>

void timer(isr_frame_t frame) {
	(void)frame;
	static char symbol = 'A';
	*((char*)0xB8000) = symbol;
	symbol = symbol == 'Z' ? 'A' : symbol + 1;
}

_Noreturn void main(void) {
	idt_init();
	isr_init();
	pic_init();
	pit_init(0xFF, timer);

	__asm__ volatile ("sti");

//	__asm__ volatile ("cli");
	while (1);
}
