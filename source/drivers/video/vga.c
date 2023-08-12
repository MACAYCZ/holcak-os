#include "vga.h"
#include <private/printf.h>
#include <string.h>
#include <port.h>

void vga_putc(char chr) {
	uint16_t cursor = vga_cursor_get();
	if (chr == '\n') {
		uint16_t row = cursor / VGA_COLS;
		if (row == VGA_ROWS-1) {
			cursor = VGA_RC(row, 0);
			vga_scroll(1);
		} else {
			cursor = VGA_RC(row+1, 0);
		}
	} else {
		VGA[cursor++] = (wchar_t)chr | VGA_DEFAULT_COLOR;
		if (cursor == VGA_ROWS * VGA_COLS) {
			cursor = VGA_RC(VGA_ROWS-1, 0);
			vga_scroll(1);
		}
	}
	vga_cursor_set(cursor);
}

void vga_printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	vga_vprintf(format, args);
	va_end(args);
}

void vga_vprintf(const char *format, va_list args) {
	vprintf_(format, vga_putc, args);
}

void vga_clear(void) {
	wmemset(VGA, L' ' | VGA_DEFAULT_COLOR, VGA_ROWS * VGA_COLS);
	vga_cursor_set(0);
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
