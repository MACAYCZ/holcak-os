#ifndef PORT_H_
#define PORT_H_
#include <stdint.h>

uint8_t port8_in(uint16_t id);
void port8_out(uint16_t id, uint8_t data);

uint16_t port16_in(uint16_t id);
void port16_out(uint16_t id, uint16_t data);

void port_wait(void);

#endif//PORT_H_
