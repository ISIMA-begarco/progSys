#ifndef INCLUDE_DEBUG_H
#define INCLUDE_DEBUG_H

#include <stdlib.h>
#include <stdio.h>

#define STRINGIFY(x) STRINGIFY_(x)
#define STRINGIFY_(x) #x

#if defined NDEBUG
#define CHECK_ERRNO(x) ((void)(x))
#else
#define CHECK_ERRNO(x) do if(!(x)) perror(__FILE__ ":" STRINGIFY(__LINE__) ": " #x), abort(); while(0)
#endif

#endif
