#ifndef DRIVERS_PPM_H_
#define DRIVERS_PPM_H_
#include <stdbool.h>
#include <stdint.h>

#define PPM_BLOCK_SIZE 0x1000

#define PPM_INFO_USABLE       0x01
#define PPM_INFO_RESERVED     0x02
#define PPM_INFO_ACPI_RECLAIM 0x03
#define PPM_INFO_NVS          0x04
#define PPM_INFO_INVALID      0x05

typedef struct {
	uint32_t start_lo;
	uint32_t start_hi;
	uint32_t size_lo;
	uint32_t size_hi;
	uint32_t type;
	uint32_t acpi;
} ppm_block_t;

typedef struct {
	uint32_t length;
	uint32_t block;
	ppm_block_t data[];
} ppm_info_t;

typedef struct ppm_node_t {
	struct ppm_node_t *next;
	uint32_t start;
	uint32_t size;
} ppm_node_t;

typedef struct ppm_pool_t {
	struct ppm_pool_t *next;
	ppm_node_t data[];
} ppm_pool_t;

bool ppm_init(ppm_info_t info);
void *ppm_alloc(uint32_t size);
bool ppm_free(void *addr, uint32_t size);

#endif//DRIVERS_PPM_H_
