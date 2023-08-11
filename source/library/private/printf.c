#include "printf.h"
#include <stdlib.h>

int vprintf_(const char *format, printf_output_t output, va_list args) {
	int size = 0;
	for (; *format; format++, size++) {
		if (*format == '%') {
			char buffer[16];
			switch (*++format) {
			case 'c':
				output(va_arg(args, char));
				break;
			case 's':
				for (char *str = va_arg(args, char*); *str; output(*str++));
				break;
			case 'd':
			case 'i':
				for (char *str = itoa(va_arg(args, ssize_t), buffer, 10); *str; output(*str++));
				break;
			case 'u':
				for (char *str = utoa(va_arg(args, ssize_t), buffer, 10); *str; output(*str++));
				break;			
			case 'o':
				for (char *str = utoa(va_arg(args, ssize_t), buffer, 8); *str; output(*str++));
				break;
			case 'x':
			case 'p':
				for (char *str = utoa(va_arg(args, ssize_t), buffer, 16); *str; output(*str++));
				break;
			}
		} else {
			output(*format);
		}
	}
	return size;
}
