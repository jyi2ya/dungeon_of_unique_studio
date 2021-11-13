#include <stdio.h>
#include <string.h>
#include <errno.h>

int cat_copy_stream_to_stdout(FILE *stream)
{
	int ch;

	errno = 0;

	while ((ch = fgetc(stream)) != EOF)
		if (putchar(ch) == EOF)
			break;

	if (errno) {
		perror("cat");
		return 1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int file_specified = 0;
	int error_occured = 0;

	while (*++argv != NULL) {
		if (strcmp(*argv, "-u") == 0) {
			setvbuf(stdout, NULL, _IONBF, 0);
			continue;
		} else if (strcmp(*argv, "-") == 0) {
			file_specified = 1;

			if (cat_copy_stream_to_stdout(stdin))
				error_occured = 1;
		} else {
			FILE *in = fopen(*argv, "r");
			file_specified = 1;

			if (in != NULL) {
				if (cat_copy_stream_to_stdout(in))
					error_occured = 1;
				fclose(in);
			} else {
				error_occured = 1;
				perror("cat");
			}
		}
	}

	if (!file_specified)
		if (cat_copy_stream_to_stdout(stdin))
			error_occured = 1;

	if (error_occured)
		return 1;
	return 0;
}
