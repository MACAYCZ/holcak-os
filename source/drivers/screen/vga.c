#include <screen/vga.h>
#include <string.h>
#include <wchar.h>
#include <port.h>

void vga_clear(void) {
	wmemset(VGA, L' ' | VGA_DEFAULT_COLOR, VGA_ROWS * VGA_COLS);
}

void vga_scroll(uint8_t rows) {
	memmove(VGA, VGA + rows * VGA_COLS, (VGA_ROWS * VGA_COLS - rows * VGA_COLS) * sizeof(uint16_t));
	memset(VGA + (VGA_ROWS * VGA_COLS - rows * VGA_COLS), 0x00, rows * VGA_COLS * sizeof(uint16_t));
}

void vga_cursor_show(uint8_t shape) {
	port8_out(0x3D4, 0x0A);
	port8_out(0x3D5, (port8_in(0x3D5) & 0xC0) | (shape >> 0x04));
	port8_out(0x3D4, 0x0B);
	port8_out(0x3D5, (port8_in(0x3D5) & 0xE0) | (shape & 0x0F));
}

void vga_cursor_hide(void) {
	port8_out(0x3D4, 0x0A);
	port8_out(0x3D5, 0x20);
}

void vga_cursor_set(uint16_t cursor) {
	port8_out(0x3D4, 0x0F);
	port8_out(0x3D5, cursor >> 0x00);
	port8_out(0x3D4, 0x0E);
	port8_out(0x3D5, cursor >> 0x08);
}

uint16_t vga_cursor_get(void) {
    uint16_t cursor = 0x00;
    port8_out(0x3D4, 0x0F);
    cursor |= port8_in(0x3D5);
    port8_out(0x3D4, 0x0E);
    cursor |= ((uint16_t)port8_in(0x3D5)) << 0x08;
    return cursor;
}
