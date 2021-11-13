#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "util.h"

int main(int argc, char *argv[])
{
	if (argc < 3) {
		puts("Too few arguments");
		return 1;
	}
	if (argv[2][strlen(argv[2]) - 1] == '/')
		argv[2][strlen(argv[2]) - 1] = '\0';
	errno = 0;
	rename(argv[1], argv[2]);

	if (errno == EISDIR || errno == EEXIST || errno == ENOTEMPTY) {
		char *buf = alloc_or_die(NULL, strlen(argv[1]) + strlen(argv[2]) + 2);
		char *infile;
		errno = 0;

		for (infile = argv[1]; *infile != '\0'; ++infile)
			;
		while (*infile != '/' && infile != argv[1])
			--infile;
		if (*infile == '/')
			++infile;

		strcpy(buf, argv[2]);
		strcat(buf, "/");
		strcat(buf, infile);

		rename(argv[1], buf);
		free(buf);
	}

	if (errno) {
		perror("mv");
		return 1;
	}
	return 0;
}
