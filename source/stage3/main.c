#include <private/define.h>
#include <stdnoreturn.h>
#include <screen/vga.h>

typedef struct _packed {
	uint8_t extensions;
	uint16_t cylinders;
	uint8_t heads;
	uint8_t sectors;
	uint8_t booting;
} disk_info_t;

typedef struct _packed {
	uint32_t base_lo;
	uint32_t base_hi;
	uint32_t length_lo;
	uint32_t length_hi;
	uint32_t type;
	uint32_t acpi;
} memory_block_t;

typedef struct _packed {
	uint32_t length;
	uint32_t block;
	memory_block_t data[];
} memory_info_t;

noreturn _cdecl void main(disk_info_t *disk, memory_info_t *memory) {
	__asm__ volatile ("cli");
	while (1);
}
