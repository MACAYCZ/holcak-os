#include "port.h"

uint8_t port8_in(uint16_t id) {
	uint8_t value;
	__asm__ volatile ("in %%dx, %%al" : "=a"(value) : "d"(id));
	return value;
}

void port8_out(uint16_t id, uint8_t value) {
	__asm__ volatile ("out %%al, %%dx" : : "a"(value), "d"(id));
}

uint16_t port16_in(uint16_t id) {
	uint16_t value;
	__asm__ volatile ("in %%dx, %%ax" : "=a"(value) : "d"(id));
	return value;
}

void port16_out(uint16_t id, uint16_t value) {
	__asm__ volatile ("out %%ax, %%dx" : : "a"(value), "d"(id));
}

uint32_t port32_in(uint16_t id) {
	uint32_t value;
	__asm__ volatile ("inl %%dx, %%eax" : "=a"(value) : "d"(id));
	return value;
}

void port32_out(uint16_t id, uint32_t value) {
	__asm__ volatile ("outl %%eax, %%dx" : : "a"(value), "d"(id));
}

void port_wait(void) {
	port8_out(0x80, 0x00);
}
