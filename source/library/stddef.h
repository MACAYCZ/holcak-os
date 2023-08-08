#pragma once
#include <stdint.h>

#define NULL ((void*)0)

#define offsetof(Type, Member) (size_t)(&((Type*)NULL)->Member)

typedef int32_t ssize_t;
typedef uint32_t size_t;
