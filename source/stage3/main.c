#include <private/define.h>
#include <stdnoreturn.h>
#include <video/vga.h>
#include <pci.h>

typedef struct __packed {
	uint8_t extensions;
	uint16_t cylinders;
	uint8_t heads;
	uint8_t sectors;
	uint8_t booting;
} disk_info_t;

typedef struct __packed {
	uint32_t base_lo;
	uint32_t base_hi;
	uint32_t length_lo;
	uint32_t length_hi;
	uint32_t type;
	uint32_t acpi;
} memory_block_t;

typedef struct __packed {
	uint32_t length;
	uint32_t block;
	memory_block_t data[];
} memory_info_t;

void pci_device_print(pci_device_t *device, uint16_t id) {
	vga_printf("Device: 0x%x\n", id);
	vga_printf("  Device id: 0x%x\n", device->device_id);
	vga_printf("  Vendor id: 0x%x\n", device->vendor_id);
	vga_printf("  Class:     0x%x\n", device->class);
	vga_printf("  Subclass:  0x%x\n", device->subclass);
	vga_printf("  Header:    0x%x\n", device->header_type);
}

noreturn __cdecl void main(disk_info_t *disk, memory_info_t *memory) {
	for (uint32_t i= 0x00; i < 0xFFFF; i += 0x08) {
		pci_device_t device;
		if (!pci_config_device(i, &device)) {
			continue;
		}
		pci_device_print(&device, i);
		if (!PCI_HAS_MULTIPLE_FUNCTIONS(device)) {
			continue;
		}
		for (uint8_t j = 0x01; j < 0x08; j++) {
			if (!pci_config_device(i | j, &device)) {
				continue;
			}
			pci_device_print(&device, i);
		}
	}
	__asm__ volatile ("cli");
	while (1);
}
