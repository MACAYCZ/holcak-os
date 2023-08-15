#pragma once
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

typedef uint16_t wchar_t;

wchar_t *wmemset(wchar_t* dst, wchar_t chr, size_t size);
