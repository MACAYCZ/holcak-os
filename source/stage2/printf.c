#include "printf.h"
#include <screen/vga.h>
#include <stdlib.h>

void printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	uint16_t cursor = vga_cursor_get();
	for (; *format; format++) {
		char buffer[16];
		if (*format == '%') {
			format++;
			switch (*format) {
			case 'c':
				VGA[cursor++] = (wchar_t)va_arg(args, char) | VGA_DEFAULT_COLOR;
				break;
			case 's':
				for (char *str = va_arg(args, char*); *str; VGA[cursor++] = (wchar_t)*str++ | VGA_DEFAULT_COLOR);
				break;
			case 'b':
				for (char *str = utoa(va_arg(args, ssize_t), buffer, 2); *str; VGA[cursor++] = (wchar_t)*str++ | VGA_DEFAULT_COLOR);
				break;
			case 'o':
				for (char *str = utoa(va_arg(args, ssize_t), buffer, 8); *str; VGA[cursor++] = (wchar_t)*str++ | VGA_DEFAULT_COLOR);
				break;
			case 'd':
			case 'i':
				for (char *str = itoa(va_arg(args, ssize_t), buffer, 10); *str; VGA[cursor++] = (wchar_t)*str++ | VGA_DEFAULT_COLOR);
				break;
			case 'u':
				for (char *str = utoa(va_arg(args, ssize_t), buffer, 10); *str; VGA[cursor++] = (wchar_t)*str++ | VGA_DEFAULT_COLOR);
				break;
			case 'x':
			case 'p':
				for (char *str = utoa(va_arg(args, ssize_t), buffer, 16); *str; VGA[cursor++] = (wchar_t)*str++ | VGA_DEFAULT_COLOR);
				break;
			case '%':
				VGA[cursor++] = L'%' | VGA_DEFAULT_COLOR;
				break;
			default:
				VGA[cursor++] = L'?' | VGA_DEFAULT_COLOR;
				break;
			}
		} else if (*format == '\n') {
			uint8_t row = cursor / VGA_COLS;
			if (row == VGA_COLS - 1) {
				cursor = VGA_RC(row, 0);
				vga_scroll(1);
			} else {
				cursor = VGA_RC(row+1, 0);
			}
		} else {
			VGA[cursor++] = (wchar_t)*format | VGA_DEFAULT_COLOR;
		}
	}
	vga_cursor_set(cursor);
	va_end(args);
}
