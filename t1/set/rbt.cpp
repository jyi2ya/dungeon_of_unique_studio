#ifndef RBT_CPP_
#define RBT_CPP_

#include <string>
#include <assert.h>

template <class T>
class rbt {
	private:
		enum color_t { red, black };

		struct node {
			T data;

			enum color_t color;
			struct node *left, *right;

			node(const T &d)
			{
				color = red;
				left = NULL;
				right = NULL;
				data = d;
			}
		};

		int cnt;
		struct node *root;

		int is_red_node(struct node *cur)
		{
			return cur != NULL && cur->color == red;
		}

		void clear_r(struct node *cur)
		{
			if (cur == NULL)
				return;
			clear_r(cur->left);
			clear_r(cur->right);
			delete cur;
		}

		void rotate_left(struct node *&cur)
		{
			struct node *t = cur->right;
			cur->right = t->left;
			t->left = cur;
			t->color = cur->color;
			cur->color = red;
			cur = t;
		}

		void rotate_right(struct node *&cur)
		{
			struct node *t = cur->left;
			cur->left = t->right;
			t->right = cur;
			t->color = cur->color;
			cur->color = red;
			cur = t;
		}

		enum color_t color_xor(enum color_t color)
		{
			return color == red ? black : red;
		}

		void flip_color(struct node *&cur)
		{
			cur->color = color_xor(cur->color);
			cur->left->color = color_xor(cur->left->color);
			cur->right->color = color_xor(cur->right->color);
		}

		void insert_fix(struct node *&cur)
		{
			if (is_red_node(cur->right)
					&& !is_red_node(cur->left))
				rotate_left(cur);
			if (is_red_node(cur->left)
					&& is_red_node(cur->left->left))
				rotate_right(cur);
			if (is_red_node(cur->left)
					&& is_red_node(cur->right))
				flip_color(cur);
		}

		void insert_r(struct node *&cur, const T &value)
		{
			if (cur == NULL) {
				cur = new node(value);
				return;
			}

			if (value < cur->data)
				insert_r(cur->left, value);
			else
				insert_r(cur->right, value);

			insert_fix(cur);
		}

		void drop_right_r(struct node *&cur, struct node *t)
		{
			if (cur == NULL) {
				cur = t;
				return;
			}
			drop_right_r(cur->right, t);
		}

		T &find_min(struct node *cur)
		{
			while (cur->left != NULL)
				cur = cur->left;
			return cur->data;
		}

		void erase_fix(struct node *&cur)
		{
			if (is_red_node(cur->right)
					&& !is_red_node(cur->left))
				rotate_left(cur);
			if (is_red_node(cur->left)
					&& is_red_node(cur->left->left))
				rotate_right(cur);
			if (is_red_node(cur->left)
					&& is_red_node(cur->right))
				flip_color(cur);
		}

		void erase_min(struct node *&cur)
		{
			if (cur->left == NULL) {
				struct node *d = cur;
				cur = cur->right;
				delete d;
				return;
			}

			if (!is_red_node(cur->left) &&
					!is_red_node(cur->left->left)) {
				cur->color = color_xor(cur->color);
				if (cur->right != NULL
					       	&& is_red_node(cur->right->left)) {
					rotate_right(cur->right);
					rotate_left(cur);
					cur->color = color_xor(cur->color);
				}
			}

			erase_min(cur->left);
			erase_fix(cur);
		}

		int erase_r(struct node *&cur, const T &value)
		{
			if (cur == NULL)
				return 0;

			if (value < cur->data) {
				assert(cur->left != NULL);
				if (!is_red_node(cur->left)
						&& !(is_red_node(cur->left->left))) {
					cur->color = color_xor(cur->color);
					if (cur->right != NULL
							&& is_red_node(cur->right->left)) {
						rotate_right(cur->right);
						rotate_left(cur);
						cur->color = color_xor(cur->color);
					}
				}
				erase_r(cur->left, value);
			} else {
				if (is_red_node(cur->left))
					rotate_right(cur);

				if (cur->data == value
						&& cur->right == NULL) {
					if (cur->left != NULL) {
						rotate_right(cur);
						erase_r(cur->right, value);
						return 1;
					} else {
						delete cur;
						cur = NULL;
						return 0;
					}
				}
				assert(cur->right != NULL);
				if (!is_red_node(cur->right) &&
						!is_red_node(cur->right->left)) {
					cur->color = color_xor(cur->color);
					//assert(cur->left != NULL);
					if (cur->left != NULL
							&& is_red_node(cur->left->left)) {
						rotate_right(cur);
						cur->color = color_xor(cur->color);
					}
				}

				if (cur->data == value) {
					cur->data = find_min(cur->right);
					erase_min(cur->right);
				} else {
					erase_r(cur->right, value);
				}
			}

			erase_fix(cur);

			return 1;
		}

		int count_r(struct node *cur, const T &value)
		{
			if (cur == NULL)
				return 0;

			if (cur->data == value)
				return 1;

			if (value < cur->data)
				return count_r(cur->left, value);
			return count_r(cur->right, value);
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
			printf("%d(%c)\n", cur->data, cur->color == red ? 'R' : 'B');

			++depth;
			print_int_r(cur->left);
			--depth;

			++depth;
			print_int_r(cur->right);
			--depth;
		}

	public:
		rbt()
		{
			cnt = 0;
			root = NULL;
		}

		~rbt()
		{
			clear();
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
			insert_r(root, value);
			root->color = black;
			++cnt;
		}

		std::string::size_type erase(const T& value)
		{
			if (count(value) == 0)
				return 0;
			if (!is_red_node(root->left)
					&& !(is_red_node(root->right)))
				root->color = red;
			erase_r(root, value);
			if (root != NULL)
				root->color = black;
			--cnt;
			return 1;
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
