#ifndef PIC_H_
#define PIC_H_
#include <stdbool.h>
#include <stdint.h>

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET (PIC1_OFFSET + 0x08)

typedef enum {
	PIC1_CMD  = 0x20,
	PIC1_DATA = 0x21,
	PIC2_CMD  = 0xA0,
	PIC2_DATA = 0xA1,
} pic_port_t;

typedef enum {
	PIC_ICW1_ICW4      = 0x01,
	PIC_ICW1_EDGE      = 0x00,
	PIC_ICW1_SINGLE    = 0x02,
	PIC_ICW1_INTERVAL4 = 0x04,
	PIC_ICW1_LEVEL     = 0x08,
	PIC_ICW1_INIT      = 0x10,
} pic_icw1_t;

typedef enum {
	PIC_ICW4_8088          = 0x00,
	PIC_ICW4_8086          = 0x01,
	PIC_ICW4_AUTO_EOI      = 0x02,
	PIC_ICW4_BUFFER_MASTER = 0x04,
	PIC_ICW4_BUFFER_SLAVE  = 0x00,
	PIC_ICW4_BUFFER        = 0x08,
	PIC_ICW4_SFNM          = 0x10,
} pic_icw4_t;

typedef enum {
	PIC_CMD_EOI = 0x20,
	PIC_CMD_IRR = 0x0A,
	PIC_CMD_ISR = 0x0B,
} pic_cmd_t;

void pic_init(void);
void pic_mask(uint8_t irq, bool mask);
void pic_mask_all(uint16_t mask);
void pic_eoi(uint8_t irq);
uint16_t pic_irr(void);
uint16_t pic_isr(void);

#endif//PIC_H_
