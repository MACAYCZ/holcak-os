#include <string.h>

char *strcpy(char *restrict dst, const char *restrict src) {
	for (size_t i = 0; src[i] != '\0'; i++) {
		dst[i] = src[i];
	}
	return dst;
}

char *strncpy(char *restrict dst, const char *restrict src, size_t size) {
	for (size_t i = 0; i < size; i++) {
		dst[i] = src[i];
	}
	return dst;
}

size_t strlen(const char *str) {
	size_t i;
	for (i = 0; str[i] != '\0'; i++);
	return i;
}

void *memchr(const void *ptr, int chr, size_t size) {
	for (size_t i = 0; i < size; i++) {
		if (((char*)ptr)[i] == chr) {
			return &((char*)ptr)[i];
		}
	}
	return NULL;
}

int memcmp(const void* lhs, const void* rhs, size_t size) {
	for (size_t i = 0; i < size; i++) {
		int value = ((char*)lhs)[i] - ((char*)rhs)[i];
		if (value != 0) {
			return value;
		}
	}
	return 0;
}

void *memset(void *dst, int chr, size_t size) {
	for (size_t i = 0; i < size; i++) {
		((char*)dst)[i] = chr;
	}
	return dst;
}

void* memcpy(void *restrict dst, const void *restrict src, size_t size) {
	for (size_t i = 0; i < size; i++) {
		((char*)dst)[i] = ((char*)src)[i];
	}
	return dst;
}

void* memmove(void* dst, const void* src, size_t size) {
	if (dst < src) {
		return memcpy(dst, src, size);
	}
	for (size_t i = size; i > 0; i--) {
		((char*)dst)[i-1] = ((char*)src)[i-1];
	}
	return dst;
}
