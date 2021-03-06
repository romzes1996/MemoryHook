#include "memhook.h"

typedef void* (*MallocType)(size_t);
typedef void (*FreeType)(void*);
typedef void* (*ReallocType)(void*, size_t);

void setGuard(unsigned char *ptr, unsigned char val)
{
	for (size_t i = 0; i < GUARD_SIZE; ++i)
		*(ptr + i) = val;
}

bool checkGuard(unsigned char *ptr, unsigned char val)
{
	for (size_t i = 0; i < GUARD_SIZE; ++i)
		if (*(ptr + i) != val)
			return false;
	return true;
}

size_t toHookSize(size_t size)
{
	return size + 2 * GUARD_SIZE + 1;
}

void* toHookPtr(void *ptr)
{
	unsigned char *tmp = (unsigned char*)ptr;
	return tmp - GUARD_SIZE - 1;
}

void* toUsrPtr(void *ptr)
{
	unsigned char *tmp = (unsigned char*)ptr;
	return tmp + GUARD_SIZE + 1;
}

void *setGuards(void *ptr, size_t size)
{
	unsigned char *tmp = (unsigned char*)ptr;
	*tmp = size;
	setGuard(tmp + 1, 0xAA); //front guard
	setGuard(tmp + 1 + GUARD_SIZE + size, 0xBB); //rear guard

	ptr = toUsrPtr(ptr);
	return ptr;
}

void* malloc(size_t size)
{
	MallocType origMalloc = (MallocType)dlsym(RTLD_NEXT, "malloc");
	void *res = (*origMalloc)(toHookSize(size));

	printf("%ld bytes were allocated\n", size);
	
	return setGuards(res, size);
}

void free(void* ptr)
{
	FreeType origFree = (FreeType)dlsym(RTLD_NEXT, "free");

	ptr = toHookPtr(ptr);

	bool flag = true;
	unsigned char *tmp = (unsigned char*)ptr;
	size_t size = *tmp;
	printf("%ld bytes were deallocated\n", size);
	flag &= checkGuard(tmp + 1, 0xAA);
	flag &= checkGuard(tmp + 1 + GUARD_SIZE + size, 0xBB); 

	(*origFree)(ptr);

	assert(flag);
}

void* realloc (void* ptr, size_t size)
{
	ReallocType origRealloc = (ReallocType)dlsym(RTLD_NEXT, "realloc");
	
	ptr = toHookPtr(ptr);
	ptr = (*origRealloc)(ptr, toHookSize(size));

	printf("%ld bytes were reallocated\n", size);
	
	return setGuards(ptr, size);
}