#ifndef DRIVERS_PMM_H_
#define DRIVERS_PMM_H_
#include <stdbool.h>
#include <stdint.h>

#define PMM_PAGE_SIZE          0x1000

#define PMM_BLOCK_USABLE       0x01
#define PMM_BLOCK_RESERVED     0x02
#define PMM_BLOCK_ACPI_RECLAIM 0x03
#define PMM_BLOCK_NVS          0x04
#define PMM_BLOCK_INVALID      0x05

typedef struct {
	uint32_t start;
	uint32_t start_;
	uint32_t size;
	uint32_t size_;
	uint32_t type;
	uint32_t acpi;
} pmm_block_t;

typedef struct {
	uint32_t length;
	uint32_t block;
	pmm_block_t data[];
} pmm_info_t;

typedef struct pmm_node_t {
	struct pmm_node_t *next;
	uint32_t start;
	uint32_t size;
} pmm_node_t;

typedef struct pmm_pool_t {
	struct pmm_pool_t *next;
	pmm_node_t data[(PMM_PAGE_SIZE - sizeof(struct pmm_pool_t*)) / sizeof(pmm_node_t)];
} pmm_pool_t;

bool pmm_init(pmm_info_t info);
void *pmm_alloc(uint32_t size);
bool pmm_free(void *ptr, uint32_t size);

#endif//DRIVERS_PMM_H_
