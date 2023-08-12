#ifndef DRIVERS_PCI_H_
#define DRIVERS_PCI_H_
#include <stdbool.h>
#include <stdint.h>

#define PCI_CONFIG_ADDRESS 0x0CF8
#define PCI_CONFIG_DATA    0x0CFC

#define PCI_INVALID_VENDOR 0xFFFF

#define PCI_BDF(B, D, F) ((((uint32_t)(B) & 0xFF) << 0x10) | (((uint32_t)(D) & 0x1F) << 0x0B) | (((uint32_t)(F) & 0x07) << 0x08))

uint32_t pci_config_read(uint16_t id, uint8_t offset);
void pci_config_write(uint16_t id, uint8_t offset, uint32_t value);

#endif//DRIVERS_PCI_H_
