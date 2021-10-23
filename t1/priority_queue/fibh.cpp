#ifndef FIBH_CPP_
#define FIBH_CPP_

#include <string>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/*
    - const_reference top() const;
    - bool empty() const;
    - size_type size() const;
    - void push(const value_type& value);
    - void pop();
    */

template <class T>
class fibh {
	private:
		struct node {
			T data;

			int degree;
			int is_marked;
			struct node *left, *right, *child;

			node()
			{
				data = 0;
				degree = 0;
				is_marked = 0;
				left = right = NULL;
				child = NULL;
			}

			node(const T &value)
			{
				data = value;
				degree = 0;
				is_marked = 0;
				left = right = NULL;
				child =  NULL;
			}
		};

		struct node *max;
		int cnt;

		void list_insert(struct node *left, struct node *mid,
				struct node *right)
		{
			assert(mid != NULL);
			if (left != NULL)
				left->right = mid;
			mid->left = left;
			if (right != NULL)
				right->left = mid;
			mid->right = right;
		}

		void list_remove(struct node *del,
			       	struct node **car, struct node **cdr)
		{
			if (del == NULL) {
				*car = *cdr = NULL;
				return;
			}

			struct node *left = del->left;
			struct node *right = del->right;

			if (left != NULL)
				left->right = right;
			if (right != NULL)
				right->left = left;
			del->left = del->right = NULL;

			if (car != NULL)
				*car = del;
			if (cdr != NULL)
				*cdr = left == NULL ? right : left;
			if (left == NULL && right == NULL)
				*cdr = NULL;
		}

		struct node *list_getlend(struct node *p)
		{
			if (p == NULL)
				return NULL;
			while (p->left != NULL)
				p = p->left;
			return p;
		}

	public:
		fibh()
		{
			max = NULL;
			cnt = 0;
		}

		bool empty() const
		{
			return cnt == 0;
		}

		std::string::size_type size() const
		{
			return cnt;
		}

		T &top() const
		{
			return max->data;
		}

		void push(const T& value)
		{
			struct node *n = new node(value);
			if (max == NULL) {
				max = n;
			} else {
				list_insert(max->left, n, max);
				if (n->data > max->data)
					max = n;
			}

			++cnt;
		}

		void pop()
		{
			struct node *del;
			struct node *res;
			struct node *p;

			assert(max != NULL);
			list_remove(max, &del, &res);

			del->child = list_getlend(del->child);
			while (del->child != NULL) {
				p = del->child->right;
				if (p != NULL)
					p->left = NULL;
				if (res == NULL) {
					res = del->child;
					res->left = res->right = NULL;
				} else {
					list_insert(res->left, del->child, res);
				}
				del->child = p;
			}

			delete del;

			struct node **deg_table = new struct node *[cnt];
			int found = 1;

			while (res != NULL && found) {
				struct node *pnext;
				found = 0;
				memset(deg_table, 0, sizeof(struct node *) * cnt);

				res = list_getlend(res);

				for (p = res->right; p != NULL; p = pnext) {
					pnext = p->right;
					if (deg_table[p->degree] == NULL) {
						deg_table[p->degree] = p;
					} else {
						found = 1;
						struct node *q = deg_table[p->degree];
						deg_table[p->degree] = NULL;
						if (p->data < q->data) {
							struct node *t = p;
							p = q;
							q = t;
						}
						list_remove(q, NULL, NULL);
						++p->degree;
						if (p->child == NULL)
							p->child = q;
						else
							list_insert(p->child, q, p->child->right);
					}
				}

			}

			delete deg_table;

			max = res;
			for (p = list_getlend(max); p != NULL; p = p->right) {
				if (p->data > max->data)
					max = p;
			}

			--cnt;
		};

		void print_int_r(struct node *p)
		{
			p = list_getlend(p);
			if (p == NULL) {
				printf("X ");
			} else {
				for (; p != NULL; p = p->right) {
					printf("%d ", p->data);
					if (p->child != NULL) {
						putchar('(');
						print_int_r(p->child);
						putchar(')');
					}
				}
			}
		}

		void print_int(void)
		{
			printf("fb(max=%d): ", max == NULL ? -1 : max->data);
			print_int_r(max);
			putchar('\n');
		}
};

#endif
