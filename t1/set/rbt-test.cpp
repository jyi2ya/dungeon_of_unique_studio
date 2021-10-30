#if 0
g++ -Wall -Werror -Wshadow -fsanitize=address -O2 -g rbt-test.cpp rand.c -o /tmp/rbt-test$$ || exit 1
/tmp/rbt-test$$
ret=$?
rm /tmp/rbt-test$$
exit $ret
#endif

#include <stdio.h>
#include <set>
#include "rand.h"
#include "rbt.cpp"
#include <stdlib.h>
#include <stdarg.h>

/*
 * 	size
 * 	insert 5
 * 	empty
 * 	insert 4
 * 	count:3
 * 	empty
 * 	size
 * 	empty
 * 	empty
 * 	erase:4
 * 	erase:4 0 1
 * 	*/
#define empty 0
#define size 1
#define clear 2
#define insert 3
#define erase 4
#define count 5

int one_special_case(void)
{
	static int case1[] = {
		size,
		insert, 5,
		empty,
		insert, 4,
		count,3,
		empty,
		size,
		empty,
		empty,
		erase, 4
	};

	static int case2[] = {
		empty,
		size,
		erase, 4,
		count, 5,
		clear,
		insert, 3,
		size,
		empty,
		insert, 4,
		insert, 2,
		size,
		erase, 4
	};

	static int case3[] = {
		insert, 5,
		empty,
		size,
		size,
		insert, 6,
		insert, 1,
		erase, 3,
		empty,
		empty,
		empty,
		erase, 5,
		empty,
		erase, 6,
		erase, 1,
	};

	static int case4[] = {
		insert, 4,
		insert, 3,
		erase, 8,
		erase, 1,
		insert, 5,
		insert, 2,
		insert, 1,
		erase, 0,
		insert, 8,
		insert, 5,
		erase, 8,
		insert, 0,
		erase, 7,
		insert, 8,
		insert, 5,
		insert, 8,
		insert, 9,
		erase, 4,
		erase, 3,
		erase, 4,
		erase, 7,
		insert, 2,
		insert, 4,
		insert, 7,
		insert, 6,
		insert, 8,
		insert, 2,
		erase, 0,
		erase, 2,
		erase, 1,
		insert, 6,
		insert, 0,
		erase, 5,
		erase, 8
	};

	if (case1 == NULL || case2 == NULL || case3 == NULL)
		return 0;

#define seq case4
	static int i = 0;

	if (i == sizeof(seq) / sizeof(*seq))
		i = 0;
	return seq[i++];
#undef seq
}

#undef empty
#undef size
#undef clear
#undef insert
#undef erase
#undef count

int rand_ie(void)
{
	return randin(3, 5);
}

//#define PRT
//#define randin(a, b) one_special_case()
//#define randin(a, b) rand_ie()

int myprint(const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	va_end(va);
	return 0;
}

#ifndef PRT
#define printf myprint
#endif

int main(void)
{
#define MAXVAL 10
	int testtime = 99999999;
	rbt<int> rb;
	std::set<int> st;

	while (testtime--) {
		switch (randin(0, 6)) {
			case 0: printf("empty\n");
			       	if (rb.empty() != st.empty()) {
					printf("empty %d %d\n", rb.empty(), st.empty());
					return 1;
				}
				break;

			case 1: printf("size\n");
			       	if (rb.size() != st.size()) {
					printf("size %lu %lu\n", rb.size(), st.size());
					return 1;
				}
				break;
			case 2: printf("clear\n");
			       	rb.clear();
				st.clear();
				break;
			case 3: {
					int val = randin(0, MAXVAL);
					printf("insert %d\n", val);
					rb.insert(val);
					st.insert(val);
				}
				break;
			case 4: {
					int val = randin(0, MAXVAL);
					printf("erase %d\n", val);
					int l = rb.erase(val);
					int s = st.erase(val);
					if (l != s) {
						printf("erase:%d %u %u\n", val, l, s);
						return 1;
					}
				}
				break;
			case 5: {
					int val = randin(0, MAXVAL);
					printf("count %d\n", val);
					int l = rb.count(val);
					int s = st.count(val);
					if (l != s) {
						printf("count:%d %u %u\n", val, l, s);
						return 1;
					}
				}
				break;
			default:
				printf("fuck\n");
				return 1;
		}
		//rb.print_int();
	}

	return 0;
}
