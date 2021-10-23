#ifndef LIST_CPP_
#define LIST_CPP_

#include <string>

template <class T>
class list {
	private:
		struct lnode {
			T data;
			struct lnode *next;

			lnode()
			{
				next = NULL;
			}

			lnode(T d)
			{
				data = d;
				next = NULL;
			}
		};

		struct lnode *head;
		int cnt;

	public:
		list(void)
		{
			head = NULL;
			cnt = 0;
		}

		bool empty(void) const
		{
			return cnt == 0;
		}

		std::string::size_type size(void) const
		{
			return cnt;
		}

		void clear(void)
		{
			while (head != NULL) {
				struct lnode *t = head;
				head = head->next;
				delete t;
			}
			cnt = 0;
		}

		void insert(const T &value)
		{
			struct lnode *p;
			if (count(value) != 0)
				return;
			p = new lnode(value);
			p->next = head;
			head = p;
			++cnt;
		}

		std::string::size_type erase(const T &value)
		{
			struct lnode **p;

			for (p = &head; *p != NULL; p = &((*p)->next))
				if ((*p)->data == value) {
					struct lnode *d = *p;
					*p = (*p)->next;
					--cnt;
					delete d;
					return 1;
				}

			return 0;
		}

		std::string::size_type count(const T &value)
		{
			int c = 0;
			struct lnode *p;

			for (p = head; p != NULL; p = p->next)
				if (p->data == value)
					++c;

			return c;
		}

		void print(void)
		{
			struct lnode *p;

			printf(">> ");
			for (p = head; p != NULL; p = p->next)
				printf("%d ", p->data);
			putchar('\n');
		}

};

#endif /* LIST_CPP_ */
