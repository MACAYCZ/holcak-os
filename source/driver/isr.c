#include "isr.h"
#include <stddef.h>
#include <pic.h>

static isr_handler_t isr_handlers[256] = {NULL};

void isr_handler_init(uint8_t id, isr_handler_t handler) {
	isr_handlers[id] = handler;
}

__attribute__((cdecl))
void isr_handler(isr_frame_t frame) {
	if (isr_handlers[frame.id] != NULL) {
		isr_handlers[frame.id](frame);
	}
	if (frame.id >= PIC1_OFFSET && frame.id < PIC1_OFFSET + 0x10) {
		pic_eoi(frame.id - PIC1_OFFSET);
	}
}
