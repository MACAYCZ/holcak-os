#include "pit.h"
#include <port.h>
#include <pic.h>

void pit_init(uint16_t count, isr_handler_t handler) {
	isr_handler_init(PIC1_OFFSET+0x00, handler);
	port8_out(PIT_CMD, PIT_CMD_MODE_3 | PIT_CMD_LO_HI_BYTE);
	port_wait();
	pit_count_set(count);
}

void pit_count_set(uint16_t count) {
	port8_out(PIT1_DATA, count >> 0);
	port_wait();
	port8_out(PIT1_DATA, count >> 8);
	port_wait();
}

uint16_t pit_count_get(void) {
	uint16_t count;
	port8_out(PIT_CMD, PIT_CMD_COUNT);
	count = port8_in(PIT1_DATA) << 0;
	count |= port8_in(PIT1_DATA) << 8;
	return count;
}
