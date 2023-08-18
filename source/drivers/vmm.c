#include "vmm.h"
#include <private/define.h>

static uint32_t __aligned(4096) vmm_dir[1024];
static uint32_t __aligned(4096) vmm_page_[1024]; // TODO: Remove!

void vmm_init_4kb(void) {
	for (uint16_t i = 0; i < 1024; i++) {
		vmm_dir[i] = VMM_DIR_ENTRY_4KB_WRITABLE | VMM_DIR_ENTRY_4KB_SUPERVISOR;
	}
#if 1 // TODO: Remove!
	for (uint16_t i = 0; i < 1024; i++) {
		vmm_page_[i] = (i * 0x1000) | VMM_TABLE_ENTRY_WRITABLE | VMM_TABLE_ENTRY_PRESENT;
	}
	vmm_dir[0] = ((uint32_t)vmm_page_) | VMM_DIR_ENTRY_4KB_WRITABLE | VMM_DIR_ENTRY_4KB_PRESENT;
#endif
	__asm__ volatile ("mov %0, %%eax; mov %%eax, %%cr3" : : "r"(vmm_dir));
	__asm__ volatile ("mov %cr0, %eax; or $0x80000001, %eax; mov %eax, %cr0");
}

/*
void vmm_init_4mb(void) {
	vmm_init_4kb();
	__asm__ volatile ("mov %cr4, %eax; or $0x00000010, %eax; mov %eax, %cr4");
}
*/

void vmm_page_set(uint32_t addr, uint32_t page) {
}
