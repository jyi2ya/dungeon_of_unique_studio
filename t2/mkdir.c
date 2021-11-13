#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	if (argv[1] == NULL) {
		puts("Too few arguments");
		return 1;
	}
	errno = 0;
	mkdir(argv[1], 0755);
	if (errno) {
		perror("mkdir");
		return 1;
	}
	return 0;
}
