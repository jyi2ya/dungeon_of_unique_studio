#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "util.h"

int main(int argc, char *argv[])
{
	int n;
	int i, j;
	struct dirent **sorted;
	int rows;
	int width = 80;
	int *column_width;

	n = scandir(".", &sorted, NULL, alphasort);

	if (n < 0) {
		perror("ls");
		return 1;
	}

	column_width = alloc_or_die(NULL, width * sizeof(int));

	for (rows = n / width + 1; rows < n; ++rows) {
		int total = 0;

		for (i = 0; i < n; i += rows) {
			int maxlen = 0;
			for (j = i; j < n && j < i + rows; ++j) {
				int len = strlen(sorted[j]->d_name) + 1;
				if (len > maxlen)
					maxlen = len;
			}
			total += maxlen;
			column_width[i / rows] = maxlen;
		}

		if (total <= width)
			break;
	}

	if (rows == n) {
		for (i = 0; i < n; ++i)
			printf("%s\n", sorted[i]->d_name);
	} else {
		for (i = 0; i < rows; ++i) {
			for (j = i; j < n; j += rows)
				printf("%-*s", column_width[j / rows], sorted[j]->d_name);
			putchar('\n');
		}
	}

	for (i = 0; i < n; ++i)
		free(sorted[i]);
	free(sorted);
	free(column_width);

	return 0;
}
