#include "wchar.h"

wchar_t *wmemset(wchar_t* dst, wchar_t chr, size_t size) {
	for (size_t i = 0; i < size; i++) {
		dst[i] = chr;
	}
	return dst;
}
