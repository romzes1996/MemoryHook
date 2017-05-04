#include "stdlib.h"

int main()
{
	int *a = (int*)malloc(10);
	free(a);
	return 0;
}