#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	/* TODO */
	if (argv[1] == NULL) {
		puts("Too few arguments");
		return 1;
	}
	errno = 0;
	unlink(argv[1]);
	if (errno) {
		perror("rm");
		return 1;
	}
	return 0;
}
