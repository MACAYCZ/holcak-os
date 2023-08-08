#pragma once
#include <stddef.h>
#include <stdarg.h>

typedef short wchar_t;

wchar_t *wmemset(wchar_t* dst, wchar_t chr, size_t size);
