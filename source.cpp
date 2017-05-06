#include "stdlib.h"
#include "stdio.h"

#define N 10

int main()
{
	int *a = (int*)malloc(N * sizeof(int));
	for (size_t i = 0; i < N; ++i)
		*(a + i) = i;

	for (size_t i = 0; i < N - 1; ++i)
		printf("%d ", *(a + N - i - 1));
	printf("%d\n", *a);

	free(a);
	return 0;
}