#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	struct timespec times[2];
	struct stat statbuf;
	if (argv[1] == NULL) {
		puts("Too few arguments");
		return 1;
	}

	errno = 0;
	stat(argv[1], &statbuf);
	times[0].tv_sec = statbuf.st_atime;
	times[1].tv_sec = statbuf.st_mtime;
	times[0].tv_nsec = statbuf.st_atim.tv_nsec;
	times[1].tv_nsec = statbuf.st_mtim.tv_nsec;
	utimensat(AT_FDCWD, argv[1], times, 0);
	if (errno) {
		if (errno == ENOENT) {
			int fd;

			errno = 0;
			fd = creat(argv[1], S_IRUSR | S_IWUSR |
					S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

			if (fd == -1) {
				perror("touch");
				return 1;
			}

			if (futimens(fd, times) == -1) {
				perror("touch");
				return 1;
			}

		} else {
			perror("touch");
			return 1;
		}
	}

	return 0;
}
