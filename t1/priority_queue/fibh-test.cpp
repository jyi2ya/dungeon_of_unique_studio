#if 0
g++ -Wall -Werror -Wshadow -O2 fibh-test.cpp rand.c -o /tmp/fibh-test$$ || exit 1
/tmp/fibh-test$$
ret=$?
rm /tmp/fibh-test$$
exit $ret
#endif

#include <stdio.h>
#include <queue>
#include "rand.h"
#include "fibh.cpp"
#include <stdlib.h>
#include <stdarg.h>

/*
    - const_reference top() const;
    - bool empty() const;
    - size_type size() const;
    - void push(const value_type& value);
    - void pop();
    */

#define top 0
#define empty 1
#define size 2
#define push 3
#define pop 4

int one_special_case(void)
{
	static int case1[] = {
			pop, /*: epmty*/
			push, 28,
			pop,
			pop, /*: epmty*/
			pop, /*: epmty*/
			push, 38,
			pop,
			pop, /*: epmty*/
			pop, /*: epmty*/
			pop, /*: epmty*/
			push, 71,
			push, 16,
			push, 84,
			pop,
			pop,
			push, 73,
			push, 10,
			pop,
			push, 16,
			push, 35,
			pop,
			pop,
			pop,
			push, 3,
			push, 0,
			pop,
			push, 70,
			pop,
			pop,
			push, 38,
			pop,
			push, 88,
			push, 12,
			push, 29,
			push, 52,
			pop,
			push, 82,
			push, 0,
			pop,
			pop,
			push, 35,
			push, 62,
			push, 60,
			push, 65,
			push, 10,
			pop,
			pop,
			pop,
			pop,
			pop,
			push, 29,
			pop,
			push, 43,
			pop,
			pop,
			pop,
			pop,
			push, 64,
			push, 2,
			push, 9,
			push, 39,
			pop,
			push, 37,
			pop,
			pop,
			pop,
			pop,
			pop,
			pop, /*: epmty*/
			push, 81,
			push, 49,
			push, 28,
			push, 10,
			push, 32,
			push, 15,
			pop,
			pop,
			pop,
			pop,
	};
	static int i = 0;

#define seq case1
	if (i == sizeof(seq) / sizeof(*seq))
		i = 0;
	return seq[i++];
#undef seq
}

#undef top
#undef empty
#undef size
#undef push
#undef pop

#define TESTTIME 99999999
//#define PRT_TBL
//#define PRT
//#define randin(a, b) one_special_case()

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
#define MAXVAL 100
	int testtime = TESTTIME;
	fibh<int> fb;
	std::priority_queue<int> pq;

	while (testtime--) {
		switch (randin(0, 5) % 2 ? 3 : 4) {
			case 0: 
				if (fb.empty()) {
					printf("pop, /*: epmty */\n");
				} else {
					printf("top,\n");
					if (fb.top() != pq.top()) {
						printf("top %d %d\n", fb.top(), pq.top());
						return 1;
					}
				}
				break;

			case 1: printf("empty,\n");
				if (fb.empty() != pq.empty()) {
					printf("empty %d %d\n", fb.empty(), pq.empty());
					return 1;
				}
				break;
			case 2: printf("size,\n");
				if (fb.size() != pq.size()) {
					printf("size %lu %lu\n", fb.size(), pq.size());
					return 1;
				}
				break;
			case 3: {
					int val = randin(0, MAXVAL);
					printf("push, %d,\n", val);
					fb.push(val);
					pq.push(val);
				}
				break;
			case 4: {
					if (fb.empty()) {
						printf("pop, /*: epmty*/\n");
					} else {
						printf("pop,\n");
						fb.pop();
						pq.pop();
					}
				}
				break;
			default:
				printf("fuck\n");
				return 1;
		}
#ifdef PRT_TBL
		fb.print_int();
#endif
	}

	return 0;
}
