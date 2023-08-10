#include "stdlib.h"
#include <stdbool.h>
#include <string.h>

int abs(int x) {
	int y = x >> (sizeof(int) * 8 - 1);
	return (x ^ y) - y;
}

long labs(long x) {
	int y = x >> (sizeof(long) * 8 - 1);
	return (x ^ y) - y;
}

char *itoa(ssize_t number, char *buffer, int base) {
	size_t size = 0;
	bool negative = false;
	if (number < 0) {
		negative = true;
		number = labs(number);
	}
	do {
		memmove(buffer+1, buffer, size++);
		*buffer = number % base + '0';
		if (*buffer > '9') {
			*buffer += 0x07;
		}
		number /= base;
	} while (number > 0);
	if (negative) {
		memmove(buffer+1, buffer, size++);
		*buffer = '-';
	}
	buffer[size] = '\0';
	return buffer;
}

char *utoa(size_t number, char *buffer, int base) {
	size_t size = 0;
	do {
		memmove(buffer+1, buffer, size++);
		*buffer = number % base + '0';
		if (*buffer > '9') {
			*buffer += 0x07;
		}
		number /= base;
	} while (number > 0);
	buffer[size] = '\0';
	return buffer;
}
