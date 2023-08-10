#include <private/define.h>
#include <stdnoreturn.h>
#include <screen/vga.h>

typedef struct _packed {
	uint8_t extensions;
	uint16_t cylinders;
	uint8_t heads;
	uint8_t sectors;
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
//	vga_clear();
	for (uint32_t i = 0; i < memory->length; i++) {
		memory_block_t *block = &memory->data[i];
		vga_printf("Memory detected: 0x%x%x = 0x%x%x (0x%x)\n", block->base_hi, block->base_lo, block->length_hi, block->length_lo, block->type);		
	}
	vga_printf("Memory info block: 0x%x\n", memory->block);
	vga_printf("Memory info length: 0x%x\n", memory->length);
	__asm__ volatile ("cli");
	while (1);
}
