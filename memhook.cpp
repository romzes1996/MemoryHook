#include "memhook.h"

typedef void* (*MallocType)(size_t);
typedef void (*FreeType)(void*);

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

void* malloc(size_t size)
{
	MallocType origMalloc = (MallocType)dlsym(RTLD_NEXT, "malloc");
	void *res = (*origMalloc)(toHookSize(size));

	printf("%ld bytes were allocated\n", size);
	unsigned char *tmp = (unsigned char*)res;
	*tmp = size;
	setGuard(tmp + 1, 0xAA); //front guard
	setGuard(tmp + 1 + GUARD_SIZE + size, 0xBB); //rear guard

	res = toUsrPtr(res);
	return res;
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