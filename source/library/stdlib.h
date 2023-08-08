#pragma once
#include <stddef.h>

int abs(int x);
long labs(long x);

char *itoa(ssize_t number, char *buffer, int base);
char *utoa(size_t number, char *buffer, int base);
