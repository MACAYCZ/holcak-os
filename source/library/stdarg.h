#ifndef STDARG_H_
#define STDARG_H_
#include "stdbool.h"

#define align(S) (((S) & ~0x03) + ((bool)((S) & 0x03) << 2))

typedef char *va_list;
#define va_start(S, A) ((S) = (char*)&(A) + align(sizeof(A)))
#define va_arg(S, T) ((S) += align(sizeof(T)), *(T*)((S) - align(sizeof(T))))
#define va_copy(S, B) ((B) = (S))
#define va_end(S)

#endif//STDARG_H_
