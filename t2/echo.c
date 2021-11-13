#include <stdio.h>

int main(int argc, char *argv[])
{
	while (*++argv != NULL)
		printf("%s%c", *argv, argv[1] == NULL ? '\n' : ' ');
	return 0;
}
