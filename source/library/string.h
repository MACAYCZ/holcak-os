#pragma once
#include <stddef.h>

char *strcpy(char *restrict dst, const char *restrict src);
char *strncpy(char *restrict dst, const char *restrict src, size_t size);
size_t strlen(const char *str);

void *memchr(const void *ptr, int chr, size_t size);
int memcmp(const void* lhs, const void* rhs, size_t size);
void *memset(void *dst, int chr, size_t size);
void* memcpy(void *restrict dst, const void *restrict src, size_t size);
void* memmove(void* dst, const void* src, size_t size);
