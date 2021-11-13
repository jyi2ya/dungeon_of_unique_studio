#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "util.h"

char *xargs_read_token(const char *sp)
{
	int ch;
	int size = 8;
	int cnt = 0;
	char *buf = alloc_or_die(NULL, size);

	do
		ch = getchar();
	while (strchr(sp, ch) != NULL);
	if (ch != EOF) {
		ungetc(ch, stdin);
		while ((ch = getchar()) != EOF && strchr(sp, ch) == NULL) {
			buf[cnt++] = ch;
			if (cnt == size) {
				size *= 2;
				buf = alloc_or_die(buf, size);
			}
		}
	}

	if (cnt == 0) {
		free(buf);
		return NULL;
	}

	buf[cnt] = '\0';
	return buf;
}

int main(int argc, char *argv[])
{
	char **paramters;
	char *token;
	int i;
	int status;

	if (argc < 2)
		++argc;
	paramters = alloc_or_die(NULL, sizeof(char *) * (argc + 1));

	for (i = 1; i < argc; ++i)
		paramters[i - 1] = argv[i];
	if (paramters[0] == NULL) {
		paramters[0] = "echo";
		paramters[1] = NULL;
	}

	while ((token = xargs_read_token(" \t\n")) != NULL) {
		int pid;

		paramters[argc - 1] = token;
		paramters[argc] = NULL;

		pid = fork();
		if (pid == -1) {
			free(token);
			perror("xargs");
			continue;
		} else if (pid == 0) {
			execvp(paramters[0], paramters);
			printf("%s not found\n", paramters[0]);
		} else {
			waitpid(pid, &status, 0);
		}

		free(token);
	}

	free(paramters);

	return 0;
}
