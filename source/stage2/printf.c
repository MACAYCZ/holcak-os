#include "printf.h"
#include <screen/vga.h>
#include <stdlib.h>

void putc(char chr, uint16_t *cursor) {
	if (chr == '\n') {
		uint8_t row = *cursor / VGA_COLS;
		if (row == VGA_ROWS - 1) {
			*cursor = VGA_RC(VGA_ROWS - 1, 0);
			vga_scroll(1);
		} else {
			*cursor = VGA_RC(row+1, 0);
		}
	} else {
		VGA[(*cursor)++] = (wchar_t)chr | VGA_DEFAULT_COLOR;
		if (*cursor == VGA_ROWS * VGA_COLS) {
			*cursor = VGA_RC(VGA_ROWS - 1, 0);
			vga_scroll(1);
		}
	}
}

void puts(const char *str, uint16_t *cursor) {
	for (; *str; putc(*str++, cursor));
}

void printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	uint16_t cursor = vga_cursor_get();
	for (; *format; format++) {
		char buffer[16];
		if (*format == '%') {
			format++;
			switch (*format) {
			case 'c': putc(va_arg(args, char), &cursor); break;
			case 's': puts(va_arg(args, char*), &cursor); break;
			case 'b': puts(utoa(va_arg(args, ssize_t), buffer, 2), &cursor); break;
			case 'o': puts(utoa(va_arg(args, ssize_t), buffer, 8), &cursor); break;
			case 'd':
			case 'i': puts(itoa(va_arg(args, ssize_t), buffer, 10), &cursor); break;
			case 'u': puts(utoa(va_arg(args, ssize_t), buffer, 10), &cursor); break;
			case 'x':
			case 'p': puts(utoa(va_arg(args, ssize_t), buffer, 16), &cursor); break;
			default:  putc('%', &cursor);
			}
		} else {
			putc(*format, &cursor);
		}
	}
	vga_cursor_set(cursor);
	va_end(args);
}
