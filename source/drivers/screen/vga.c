#include "vga.h"
#include <string.h>

void vga3_clear(void) {
	memset(VGA3, 0x00, VGA3_ROW * VGA3_COL * sizeof(vga3_cell_t));
}

void vga3_scroll(uint8_t row) {
	memmove(VGA3, VGA3 + row * VGA3_COL, (VGA3_ROW * VGA3_COL - row * VGA3_COL) * sizeof(vga3_cell_t));
	memset(VGA3 + (VGA3_ROW * VGA3_COL - row * VGA3_COL), 0x00, row * VGA3_COL * sizeof(vga3_cell_t));
}

/*
void vga3_cursor(bool enable);
void vga3_cursor_set(vga3_cursor_t cursor);
vga3_cursor_t vga3_cursor_get(void);
*/
