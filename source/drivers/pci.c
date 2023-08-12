#include "pci.h"
#include <stddef.h>
#include <port.h>

#define PCI_HEADER_START (offsetof(pci_device_t, header))
#define PCI_DEVICE_SIZE(Type) (((Type) & 0x03) == 0x02 ? 0x48 : 0x40)

uint32_t pci_config_read(uint16_t id, uint8_t offset) {
	port32_out(PCI_CONFIG_ADDRESS, ((uint32_t)id << 0x08) | ((uint32_t)offset) | (0x01 << 0x1F));
	return port32_in(PCI_CONFIG_DATA);
}

void pci_config_write(uint16_t id, uint8_t offset, uint32_t value) {
	port32_out(PCI_CONFIG_ADDRESS, ((uint32_t)id << 0x08) | ((uint32_t)offset) | (0x01 << 0x1F));
	port32_out(PCI_CONFIG_DATA, value);
}

bool pci_config_device(uint16_t id, pci_device_t *device) {
	((uint32_t*)device)[0x00] = pci_config_read(id, 0x00);
	if (device->vendor_id == PCI_INVALID_VENDOR) {
		return false;
	}
	for (uint8_t offset = 0x04; offset < PCI_HEADER_START; offset += 0x04) {
		((uint32_t*)device)[offset >> 0x02] = pci_config_read(id, offset);
	}
	for (uint8_t offset = PCI_HEADER_START; offset < PCI_DEVICE_SIZE(device->header_type); offset += 0x04) {
		((uint32_t*)device)[offset >> 0x02] = pci_config_read(id, offset);
	}
	return true;
}
