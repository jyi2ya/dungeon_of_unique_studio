#if 0
#g++ -Wall -Werror -Wshadow -O2 list-test.cpp rand.c -o /tmp/list-test$$ || exit 1
g++ list-test.cpp rand.c -o /tmp/list-test$$ || exit 1
/tmp/list-test$$
ret=$?
rm /tmp/list-test$$
exit $ret
#endif

#include <stdio.h>
#include <set>
#include "rand.h"
#include "list.cpp"
#include <stdlib.h>

#define MAXVAL 10

/*
    - bool empty() const;
    - size_type size() const;
    - void clear();
    - void insert(const value_type& value);
    - size_type erase(const value_type& value);
    - size_type count(const value_type& value);
 *	size        
 * 	erase:5     
 * 	erase:6     
 * 	count:7     
 * 	insert 3    
 * 	count:5     
 * 	empty       
 * 	size        
 * 	clear       
 * 	count:7     
 * 	insert 5    
 * 	erase:2     
 * 	insert 6    
 * 	erase:7     
 * 	erase:5     
 * 	erase:5 1 0 
 */

//#define randin(a, b) one_special_case()

int one_special_case(void)
{
	static int seq[] = {
		1,
		4, 5,
		4, 6,
		5, 7,
		3, 3,
		5, 5,
		0,
		1,
		2,
		5, 7,
		3, 5,
		4, 2,
		3, 6,
		4, 7,
		4, 5
	};
	static int i = 0;
	if (i == sizeof(seq) / sizeof(seq[0]))
		i = 0;
	return seq[i++];
}

int main(void)
{
	int testtime = 99999999;
	list<int> lst;
	std::set<int> st;

	/*
    - bool empty() const;
    - size_type size() const;
    - void clear();
    - void insert(const value_type& value);
    - size_type erase(const value_type& value);
    - size_type count(const value_type& value);
    */

	while (testtime--) {
		switch (randin(0, 6)) {
			case 0: //printf("empty\n");
			       	if (lst.empty() != st.empty()) {
					//printf("empty %d %d\n", lst.empty(), st.empty());
					return 1;
				}
				break;

			case 1: //printf("size\n");
			       	if (lst.size() != st.size()) {
					//printf("size %lu %lu\n", lst.size(), st.size());
					return 1;
				}
				break;
			case 2: //printf("clear\n");
			       	lst.clear();
				st.clear();
				break;
			case 3: {
					int val = randin(0, MAXVAL);
					//printf("insert %d\n", val);
					lst.insert(val);
					st.insert(val);
				}
				break;
			case 4: {
					int val = randin(0, MAXVAL);
					//printf("erase:%d\n", val);
					int l = lst.erase(val);
					int s = st.erase(val);
					if (l != s) {
						//printf("erase:%d %u %u\n", val, l, s);
						return 1;
					}
				}
				break;
			case 5: {
					int val = randin(0, MAXVAL);
					//printf("count:%d\n", val);
					int l = lst.count(val);
					int s = st.count(val);
					if (l != s) {
						//printf("count:%d %u %u\n", val, l, s);
						return 1;
					}
				}
				break;
			default:
				//printf("fuck\n");
				return 1;
		}
	}

	return 0;
}
