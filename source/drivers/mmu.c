#include "mmu.h"
#include <private/define.h>

static uint32_t __aligned(4096) mmu_dir[1024];
static uint32_t __aligned(4096) mmu_page_[1024]; // TODO: Remove!

void mmu_init_4kb(void) {
	for (uint16_t i = 0; i < 1024; i++) {
		mmu_dir[i] = MMU_DIR_ENTRY_4KB_WRITABLE | MMU_DIR_ENTRY_4KB_SUPERVISOR;
	}
#if 1 // TODO: Remove!
	for (uint16_t i = 0; i < 1024; i++) {
		mmu_page_[i] = (i * 0x1000) | MMU_TABLE_ENTRY_WRITABLE | MMU_TABLE_ENTRY_PRESENT;
	}
	mmu_dir[0] = ((uint32_t)mmu_page_) | MMU_DIR_ENTRY_4KB_WRITABLE | MMU_DIR_ENTRY_4KB_PRESENT;
#endif
	__asm__ volatile ("mov %0, %%eax; mov %%eax, %%cr3" : : "r"(mmu_dir));
	__asm__ volatile ("mov %cr0, %eax; or $0x80000001, %eax; mov %eax, %cr0");
}

/*
void mmu_init_4mb(void) {
	mmu_init_4kb();
	__asm__ volatile ("mov %cr4, %eax; or $0x00000010, %eax; mov %eax, %cr4");
}
*/

void mmu_page_set(uint32_t addr, uint32_t page) {
}
