#include <stdio.h>
#include <string.h>
#include <limits.h>

void randfill(void *ptr, size_t size)
{
	static FILE *fp = NULL;

	if (fp == NULL)
		if ((fp = fopen("/dev/urandom", "r")) == NULL) {
			perror("readfill:");
			return;
		}

	fread(ptr, size, 1, fp);
}

#define define_rand_basic_types(func, type) \
	type rand ## func (void) { \
		type r; \
		randfill(&r, sizeof(r)); \
		return r; \
	}

define_rand_basic_types(int, int)
define_rand_basic_types(long, long)
define_rand_basic_types(uint, unsigned int)
define_rand_basic_types(ulong, unsigned long)

#undef define_rand_basic_types

int randpint(void)
{
	return (int)((unsigned int)(randint()) & (~(1U << (sizeof(int) * 8 - 1))));
}

double rand01(void)
{
	return (double)randulong() / ULONG_MAX;
}

/* [l, r) */
int randin(int l, int r)
{
	return l + rand01() * (r - l);
}

/*
int main(void)
{
	int i;
	for (i = 0; i < 99999999; ++i)
		if (randpint() < 0)
			return 1;
	for (i = 0; i < 99999999; ++i)
		if (randin(0, 6) >= 6 || randin(0, 6) < 0)
			return 1;
	return 0;
}
*/
