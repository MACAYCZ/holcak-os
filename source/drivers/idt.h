#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	IDT_GATE_TASK    = 0x05,
	IDT_GATE_INT_16  = 0x06,
	IDT_GATE_TRAP_16 = 0x07,
	IDT_GATE_INT_32  = 0x0E,
	IDT_GATE_TRAP_32 = 0x0F,
	IDT_RING_0       = 0x00,
	IDT_RING_1       = 0x20,
	IDT_RING_2       = 0x40,
	IDT_RING_3       = 0x60,
	IDT_PRESENT      = 0x80,
} idt_flags_t;

typedef struct {
	uint16_t base_low;
	uint16_t segment;
	uint8_t  reserved;
	uint8_t  flags;
	uint16_t base_high;
} __attribute__((packed))
idt_gate_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed))
idt_descriptor_t;

void idt_init(void);
void idt_gate_init(uint8_t id, void(*base)(void), uint8_t flags);
