#ifndef DRIVERS_PCI_H_
#define DRIVERS_PCI_H_
#include <private/define.h>
#include <stdbool.h>
#include <stdint.h>

#define PCI_CONFIG_ADDRESS 0x0CF8
#define PCI_CONFIG_DATA    0x0CFC

#define PCI_INVALID_VENDOR 0xFFFF

#define PCI_BDF(Bus, Device, Function) ((((uint32_t)(Bus) & 0xFF) << 0x10) | (((uint32_t)(Device) & 0x1F) << 0x0B) | (((uint32_t)(Function) & 0x07) << 0x08))
#define PCI_HAS_MULTIPLE_FUNCTIONS(Device) ((bool)((Device).header_type & 0x80))

typedef enum {
	PCI_HEADER_GENERAL_DEVICE,
	PCI_HEADER_PCI_TO_PCI,
	PCI_HEADER_PCI_TO_CARDBUS,
} pci_header_type_t;

typedef union {
	struct __packed {
		uint32_t base_address_0;
		uint32_t base_address_1;
		uint32_t base_address_2;
		uint32_t base_address_3;
		uint32_t base_address_4;
		uint32_t base_address_5;
		uint32_t cardbus_cis;
		uint16_t subsystem_vendor_id;
		uint16_t subsystem_id;
		uint32_t rom_address;
		uint8_t capabilities;
		uint8_t reserved[0x07];
		uint8_t interrupt_line;
		uint8_t interrupt_pin;
		uint8_t min_grant;
		uint8_t max_latency;
	} x0;
	struct __packed {
		uint32_t base_address_0;
		uint32_t base_address_1;
		uint8_t primary_bus;
		uint8_t secondary_bus;
		uint8_t subordinate_bus;
		uint8_t secondary_latency;
		uint8_t io_base_lo;
		uint8_t io_limit_lo;
		uint16_t secondary_status;
		uint16_t memory_base;
		uint16_t memory_limit;
		uint16_t prefetch_memory_base_lo;
		uint16_t prefetch_memory_limit_lo;
		uint32_t prefetch_memory_base_hi;
		uint32_t prefetch_memory_limit_hi;
		uint16_t io_base_hi;
		uint16_t io_limit_hi;
		uint8_t capability;
		uint8_t reserved[0x03];
		uint32_t rom_address;
		uint8_t interrupt_line;
		uint8_t interrupt_pin;
		uint16_t bridge_control;
	} x1;
	struct __packed {
		uint32_t socket;
		uint8_t capabilities_offset;
		uint8_t reserved[0x01];
		uint16_t secondary_status;
		uint8_t pci_bus_number;
		uint8_t cardbus_bus_number;
		uint8_t subordinate_bus_number;
		uint8_t cardbus_latency;
		uint32_t memory_base_0;
		uint32_t memory_limit_0;
		uint32_t memory_base_1;
		uint32_t memory_limit_1;
		uint32_t io_base_0;
		uint32_t io_limit_0;
		uint32_t io_base_1;
		uint32_t io_limit_1;
		uint8_t interrupt_line;
		uint8_t interrupt_pin;
		uint16_t bridge_control;
		uint16_t subsystem_device_id;
		uint16_t subsystem_vendor_id;
		uint32_t legacy_base;
	} x2;
} pci_header_t;

typedef struct __packed {
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t command_reg;
	uint16_t status_reg;
	uint8_t revision_id;
	uint8_t prog_if;
	uint8_t subclass;
	uint8_t class;
	uint8_t cache_size;
	uint8_t latency;
	uint8_t header_type;
	uint8_t bist;
	pci_header_t header;
} pci_device_t;

uint32_t pci_config_read(uint16_t id, uint8_t offset);
void pci_config_write(uint16_t id, uint8_t offset, uint32_t value);
bool pci_config_device(uint16_t id, pci_device_t *device);

#endif//DRIVERS_PCI_H_
