#ifndef ISR_H_
#define ISR_H_
#include "isrg.h"

typedef struct {
	uint32_t ds, edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t id, error;
	uint32_t eip, cs, flags, user_esp, ss;
} __attribute__((packed))
isr_frame_t;

typedef void(*isr_handler_t)(isr_frame_t);
void isr_handler_init(uint8_t id, isr_handler_t handler);

__attribute__((cdecl))
void isr_handler(isr_frame_t frame);

#endif//ISR_H_
