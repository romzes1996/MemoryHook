#include "memhook.h"

typedef void* (*MallocType)(size_t);
typedef void (*FreeType)(void *);

void* malloc(size_t size)
{
	printf("Hello Malloc!\n");
	MallocType origMalloc = (MallocType)dlsym(RTLD_NEXT, "malloc");
	return (*origMalloc)(size);
}

void free(void* ptr)
{
	printf("Hello Free!\n");
	FreeType origFree = (FreeType)dlsym(RTLD_NEXT, "free");
	(*origFree)(ptr);
}