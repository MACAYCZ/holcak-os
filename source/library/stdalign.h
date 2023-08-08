#pragma once

#define __alignas_is_defined 1
#define __alignof_is_defined 1

#define alignas(X) _Alignas(X)
#define alignof(X) _Alignof(X)
