#include <stdio.h>
#include <stdlib.h>

#include <pwd.h>
#include <unistd.h>

#include "util.h"

int main(int argc, char *argv[])
{
	int max = pathconf(".", _PC_PATH_MAX);
	char *p;
	char *buf;
	if (max == -1)
		max = 4096;
	buf = alloc_or_die(NULL, max);
	p = getcwd(buf, max);
	if (p == NULL) {
		perror("pwd");
		return 1;
	} else {
		printf("%s\n", buf);
	}
	free(buf);
	return 0;
}
