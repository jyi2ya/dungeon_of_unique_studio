#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int i;
	int status = 0;

	if (argc < 2) {
		puts("Too few arguments");
		return 1;
	}

	for (i = 1; i < argc; ++i) {
		errno = 0;
		kill(atoi(argv[i]), SIGTERM);
		if (errno) {
			perror("kill");
			status = 1;
		}
	}

	return status;
}
