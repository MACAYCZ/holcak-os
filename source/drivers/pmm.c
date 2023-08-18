#include "pmm.h"
#include <stddef.h>
#include <string.h>

static pmm_pool_t *pmm_pool = NULL;

bool pmm_init(pmm_info_t info) {
	for (uint32_t i = 0; i < info.length; i++) {
		if (info.data[i].type == PMM_BLOCK_USABLE && info.data[i].size > PMM_PAGE_SIZE) {
			memset(pmm_pool = info.data[i].start, 0x00, PMM_PAGE_SIZE);
			pmm_pool->data[0] = (pmm_node_t){NULL, info.data[i].start, PMM_PAGE_SIZE};
			info.data[i].start += PMM_PAGE_SIZE;
			info.data[i].size -= PMM_PAGE_SIZE;
			return true;
		}
	}
	return false;
}

void *pmm_alloc(uint32_t size) {
	
}

bool pmm_free(void *ptr, uint32_t size);
