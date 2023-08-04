#ifndef PIT_H_
#define PIT_H_
#include "isr.h"

#define PIT_COUNT_MAX 1193180

typedef enum {
	PIT1_DATA = 0x40,
	PIT2_DATA = 0x41,
	PIT3_DATA = 0x42,
	PIT_CMD   = 0x43,
} pit_port_t;

typedef enum {
	PIT_CMD_BINARY_MODE = 0x00,
	PIT_CMD_BCD_MODE    = 0x01,
	PIT_CMD_MODE_0      = 0x00,
	PIT_CMD_MODE_1      = 0x02,
	PIT_CMD_MODE_2      = 0x04,
	PIT_CMD_MODE_3      = 0x06,
	PIT_CMD_MODE_4      = 0x08,
	PIT_CMD_MODE_5      = 0x0A,
	PIT_CMD_COUNT       = 0x00,
	PIT_CMD_LO_BYTE     = 0x10,
	PIT_CMD_HI_BYTE     = 0x20,
	PIT_CMD_LO_HI_BYTE  = 0x30,
	PIT_CMD_CHANNEL_0   = 0x00,
	PIT_CMD_CHANNEL_1   = 0x40,
	PIT_CMD_CHANNEL_2   = 0x80,
	PIT_CMD_READ_BACK   = 0xC0,
} pit_cmd_t;

void pit_init(uint16_t count, isr_handler_t handler);
void pit_count_set(uint16_t count);
uint16_t pit_count_get(void);

#endif//PIT_H_
