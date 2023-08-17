#include "ppm.h"
#include <stddef.h>

static ppm_node_t *ppm_data = NULL;

bool ppm_init(ppm_info_t info) {
	for (uint32_t i = 0; i < info.length; i++) {
		if (info.data[i].type == PPM_BLOCK_USABLE && info.data[i].size >= PPM_BLOCK_SIZE) {
			ppm_data = info.data[i].start;
			info.data[i].start += PPM_BLOCK_SIZE;
			info.data[i].size -= PPM_BLOCK_SIZE;
		}
	}
}

void *ppm_alloc(uint32_t size);
bool ppm_free(void *addr, uint32_t size);
