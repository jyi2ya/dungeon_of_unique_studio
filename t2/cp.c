#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "util.h"

int main(int argc, char *argv[])
{
	int ch;
	FILE *in, *out;
	if (argc < 3) {
		puts("Too few arguments");
		return 1;
	}

	if (argv[2][strlen(argv[2]) - 1] == '/')
		argv[2][strlen(argv[2]) - 1] = '\0';

	in = fopen(argv[1], "rb");
	if (in == NULL) {
		perror("cp");
		return 1;
	}

	out = fopen(argv[2], "wb");
	if (out == NULL  && errno == EISDIR) {
		char *buf = alloc_or_die(NULL, strlen(argv[1]) + strlen(argv[2]) + 2);
		char *infile;
		for (infile = argv[1]; *infile != '\0'; ++infile)
			;
		while (*infile != '/' && infile != argv[1])
			--infile;
		if (*infile == '/')
			++infile;

		strcpy(buf, argv[2]);
		strcat(buf, "/");
		strcat(buf, infile);

		out = fopen(buf, "wb");
		free(buf);
	}

	if (out == NULL) {
		perror("cp");
		fclose(in);
		return 1;
	}

	while ((ch = fgetc(in)) != EOF)
		fputc(ch, out);

	fclose(in);
	fclose(out);
	return 0;
}
