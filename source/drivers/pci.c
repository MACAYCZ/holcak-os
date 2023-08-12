#include "pci.h"
#include <port.h>

uint32_t pci_config_read(uint16_t id, uint8_t offset) {
	// NOTE: First 2 bits of offset has to be zero!
	// NOTE: https://wiki.osdev.org/PCI#Configuration_Space_Access_Mechanism_.231
	port32_out(PCI_CONFIG_ADDRESS, ((uint32_t)id << 0x08) | ((uint32_t)offset) | (0x01 << 0x1F));
	return port32_in(PCI_CONFIG_DATA);
}

void pci_config_write(uint16_t id, uint8_t offset, uint32_t value) {
	// NOTE: First 2 bits of offset has to be zero!
	port32_out(PCI_CONFIG_ADDRESS, ((uint32_t)id << 0x08) | ((uint32_t)offset) | (0x01 << 0x1F));
	port32_out(PCI_CONFIG_DATA, value);
}
