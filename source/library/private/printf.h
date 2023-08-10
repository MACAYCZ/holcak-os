#pragma once
#include <stdarg.h>

typedef void(*printf_output_t)(char);

int vprintf_(const char *format, printf_output_t output, va_list args);
