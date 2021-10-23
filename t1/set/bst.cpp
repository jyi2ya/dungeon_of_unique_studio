#ifndef RBT_CPP_
#define RBT_CPP_

#include <string>

template <class T>
class rbt {
	private:
		struct node;

		struct edge {
			enum {
			       	red, black
			} color;
			struct node *dst;

			edge()
			{
				color = red;
				dst = NULL;
			}

			edge(int c, struct node *d)
			{
				color = c;
				dst = d;
			}
		};

		struct node {
			T data;

			struct edge left;
			struct edge right;
			struct edge father;

			node(const T &d)
			{
				data = d;
			}
		};

		int cnt;
		struct node *root;

		void clear_r(struct node *cur)
		{
			if (cur == NULL)
				return;
			clear_r(cur->left.dst);
			clear_r(cur->right.dst);
			delete cur;
		}

		void insert_r(struct node **cur, const T &value)
		{
			if ((*cur) == NULL) {
				*cur = new node(value);
				++cnt;
				return;
			}

			if (value < (*cur)->data) {
				insert_r(&((*cur)->left.dst), value);
				(*cur)->left.dst->father.dst = *cur;
			} else {
				insert_r(&((*cur)->right.dst), value);
				(*cur)->right.dst->father.dst = *cur;
			}
		}

		void drop_right_r(struct node **cur, struct node *t)
		{
			if (*cur == NULL) {
				*cur = t;
				return;
			}
			drop_right_r(&((*cur)->right.dst), t);
		}

		int erase_r(struct node **cur, const T &value)
		{
			if ((*cur) == NULL)
				return 0;

			if ((*cur)->data == value) {
				struct node *d = *cur;
				*cur = d->left.dst;
				drop_right_r(cur, d->right.dst);
				delete d;
				--cnt;
				return 1;
			}

			if (value < (*cur)->data)
				return erase_r(&((*cur)->left.dst), value);
			return erase_r(&((*cur)->right.dst), value);
		}

		int count_r(struct node *cur, const T &value)
		{
			if (cur == NULL)
				return 0;

			if (cur->data == value)
				return 1;

			if (value < cur->data)
				return count_r(cur->left.dst, value);
			return count_r(cur->right.dst, value);
		}

		int depth;

		void print_int_r(struct node *cur)
		{
			int i;
			if (cur == NULL) {
				for (i = 0; i < depth; ++i)
					printf("--");
				printf("X\n");
				return;
			}

			for (i = 0; i < depth; ++i)
				printf("--");
			printf("%d\n", cur->data);

			++depth;
			print_int_r(cur->left.dst);
			--depth;

			++depth;
			print_int_r(cur->right.dst);
			--depth;
		}

	public:
		rbt()
		{
			cnt = 0;
			root = NULL;
		}

		bool empty() const
		{
			return cnt == 0;
		}

		std::string::size_type size() const
		{
			return cnt;
		}

		void clear()
		{
			clear_r(root);
			root = NULL;
			cnt = 0;
		}

		void insert(const T& value)
		{
			if (count(value) != 0)
				return;
			insert_r(&root, value);
		}

		std::string::size_type erase(const T& value)
		{
			return erase_r(&root, value);
		}

		std::string::size_type count(const T& value)
		{
			return count_r(root, value);
		}

		void print_int(void)
		{
			puts("==== tree ====");
			depth = 0;
			print_int_r(root);
		}
};

#endif
