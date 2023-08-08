#include <idt.h>
#include <isr.h>

static idt_gate_t idt_table[256];
static idt_descriptor_t idt_descriptor;

void idt_init(void) {
	idt_descriptor = (idt_descriptor_t){sizeof(idt_table)-1, (uint32_t)idt_table};
	__asm__ volatile ("lidtl (%0)" : : "r" (&idt_descriptor));
}

void idt_gate_init(uint8_t id, void(*base)(void), uint8_t flags) {
	idt_table[id] = (idt_gate_t){(uint32_t)base >> 0, 0x08, 0x00, flags, (uint32_t)base >> 16};
}
