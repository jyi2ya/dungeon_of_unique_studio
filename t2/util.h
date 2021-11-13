#ifndef UTIL_H_
#define UTIL_H_

void *alloc_or_die(void *p, size_t size);
char *strndup_or_die(char *head, int size);
int get_terminal_size(int *width, int *height);

#endif /* UTIL_H_ */
