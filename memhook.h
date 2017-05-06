#ifndef MEMHOOK_H
#define MEMHOOK_H

#include <dlfcn.h>
#include <stdio.h>
#include <assert.h>

#define GUARD_SIZE 8

extern "C" void* malloc(size_t);
extern "C" void free(void*);

#endif //MEMHOOK_H