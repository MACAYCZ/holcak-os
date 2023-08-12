#pragma once
#include <stdint.h>

uint8_t port8_in(uint16_t id);
void port8_out(uint16_t id, uint8_t value);

uint16_t port16_in(uint16_t id);
void port16_out(uint16_t id, uint16_t value);

uint32_t port32_in(uint16_t id);
void port32_out(uint16_t id, uint32_t value);

void port_wait(void);
