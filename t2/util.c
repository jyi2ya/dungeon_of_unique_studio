#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "util.h"

extern void *alloc_or_die(void *p, size_t size)
{
	p = realloc(p, size);
	if (p == NULL) {
		fprintf(stderr, "%s\n", strerror(errno));
		exit(1);
	}
	return p;
}

extern char *strndup_or_die(char *head, int size)
{
	char *buf = NULL;
	buf = alloc_or_die(buf, size + 1);
	strncpy(buf, head, size);
	buf[size] = '\0';
	return buf;
}

extern int get_terminal_size(int *width, int *height)
{
	return 0;
}
