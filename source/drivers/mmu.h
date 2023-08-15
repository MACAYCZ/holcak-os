#ifndef DRIVERS_MMU_H_
#define DRIVERS_MMU_H_
#include <stdint.h>

// NOTE: I have to enable paging in 16-bit real mode!

typedef struct {
	uint32_t present : 1;
	uint32_t writable : 1;
	uint32_t user : 1;
	uint32_t write_through : 1;
	uint32_t cashe_disable : 1;
	uint32_t accessed : 1;
	uint32_t unused_0 : 1;
	uint32_t page_size : 1;
	uint32_t unused_1 : 4;
	uint32_t address : 20;
} mmu_directory_entry_4kb_t;

typedef struct {
	uint32_t present : 1;
	uint32_t writable : 1;
	uint32_t user : 1;
	uint32_t write_through : 1;
	uint32_t cashe_disable : 1;
	uint32_t accessed : 1;
	uint32_t dirty : 1;
	uint32_t page_size : 1;
	uint32_t global : 1;
	uint32_t unused_0 : 3;
	uint32_t attribute_table : 1;
	uint32_t address_hi : 8;
	uint32_t reserved : 1;
	uint32_t address_lo : 10;
} mmu_directory_entry_4mb_t;

typedef struct {
	uint32_t present : 1;
	uint32_t writable : 1;
	uint32_t user : 1;
	uint32_t write_through : 1;
	uint32_t cashe_disable : 1;
	uint32_t accessed : 1;
	uint32_t dirty : 1;
	uint32_t attribute_table : 1;
	uint32_t global : 1;
	uint32_t unused_0 : 3;
	uint32_t address : 20;
} mmu_table_entry_t;

#endif//DRIVERS_MMU_H_
