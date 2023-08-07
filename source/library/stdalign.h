#ifndef STDALIGN_H_
#define STDALIGN_H_

#define __alignas_is_defined 1
#define __alignof_is_defined 1

#define alignas(X) _Alignas(X)
#define alignof(X) _Alignof(X)

#endif//STDALIGN_H_