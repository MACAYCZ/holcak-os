#include <port.h>

uint8_t port8_in(uint16_t id) {
	uint8_t data;
	__asm__ volatile ("in %%dx, %%al" : "=a" (data) : "d" (id));
	return data;
}

void port8_out(uint16_t id, uint8_t data) {
	__asm__ volatile ("out %%al, %%dx" : : "a" (data), "d" (id));
}

uint16_t port16_in(uint16_t id) {
	uint16_t data;
	__asm__ volatile ("in %%dx, %%ax" : "=a" (data) : "d" (id));
	return data;
}

void port16_out(uint16_t id, uint16_t data) {
	__asm__ volatile ("out %%ax, %%dx" : : "a" (data), "d" (id));
}

void port_wait(void) {
	port8_out(0x80, 0);
}
