#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <string>
#include <utility>
#include <stack>
#include <queue>

namespace implementation {

	template <typename key_type, class compare = std::less<key_type> >
	class AVL_tree {

	private:

		struct Node {

			key_type data;
			Node* left;
			Node* right;
			// char meaning a 1 byte int in this case
			char height;
			Node(const key_type& value) :
				data(value),
				right(nullptr),
				left(nullptr),
				height(-1) {}
			Node(key_type&& value) :
				data(std::move(value)),
				right(nullptr),
				left(nullptr),
				height(-1) {}
		};

	private:
		const compare comp;
		size_t size_;
		Node* head;

	private:

		template<typename key_type>
		class forward_iterator {
			// to access private constructors
			friend AVL_tree;

			using iterator_category = std::forward_iterator_tag;
			using pointer = Node * ;
			using reference = Node & ;
			using value = key_type;
			// constructors
		public:

			forward_iterator() : current(nullptr) {}

			forward_iterator(const forward_iterator& source) :
				current(source.current),
				parents(source.parents) {}

			forward_iterator& operator=(const forward_iterator& rhs) {
				current = rhs.current;
				parents = rhs.parents;
				return *this;
			}
			bool operator==(const forward_iterator& rhs) {
				return this->current == rhs.current;
			}
			bool operator!=(const forward_iterator& rhs) {
				return this->current != rhs.current;
			}
		private:
			forward_iterator(pointer c, std::stack<pointer> p) : current(c), parents(p) {}
		public:

			forward_iterator & operator++() {
				if (current == nullptr) {
					throw std::exception("error advancing end iterator");
				}
				if (current->right != nullptr) {
					current = current->right;
					while (current->left != nullptr) {
						parents.push(current);
						current = current->left;
					}

				}
				else {
					if (parents.empty()) {
						current = nullptr;
					}
					else {
						current = parents.top();
						parents.pop();
					}
				}
				return *this;
			}
			forward_iterator operator++(int) {
				forward_iterator temp(this);
				operator++();
				return temp;
			}
			bool operator ==(const forward_iterator& rhs) const {
				return current == rhs.current;
			}
			bool operator !=(const forward_iterator& rhs) const {
				return current != rhs.current;
			}
			key_type& operator*() const {
				if (current == nullptr) {
					throw std::out_of_range("error dereferencing an invalid iterator");
				}
				return current->data;
			}
			key_type* operator->() const {
				if (current == nullptr) {
					throw std::out_of_range("error dereferencing an invalid iterator");
				}
				return &current->data;
			}

		private:
			pointer current;
			std::stack<pointer> parents;
		};

	public:
		using iterator = forward_iterator<key_type>;
		using const_iterator = forward_iterator<const key_type>;

	public:
		AVL_tree() : head(nullptr), size_(0) {}

		AVL_tree(AVL_tree&& oth) : head(nullptr), size_(0) {
			std::swap(this->head, oth.head);
			std::swap(this->size_, oth.size_);
		}

		AVL_tree(const AVL_tree& oth) : size_(0) {
			// TODO: make this more efficient
			this->operator=(oth);
		}

		AVL_tree operator=(const AVL_tree& oth) {
			this->clear();
			for (auto elem : oth) {
				insert(elem);
			}
		}

		AVL_tree& operator=(AVL_tree&& oth) {
			clear();
			std::swap(this->head, oth.head);
			std::swap(this->size_, oth.size_);
		}

		AVL_tree(const std::initializer_list<key_type>& list) {
			for (auto elem : list) {
				insert(elem);
			}
		}
		AVL_tree(std::initializer_list<key_type>&& list) {
			for (auto elem : list) {
				insert(std::move(elem));
			}
		}
		~AVL_tree() {
			this->clear();
		}

		bool empty() const {
			return head == nullptr;
		}
		size_t size() const {
			return size_;
		}

		const_iterator cbegin() const {
			if (head == nullptr) {
				return cend();
			}
			std::stack<Node*> s;
			Node* min = minimum(head, s);

			return const_iterator(min, std::move(s));
		}
		const_iterator cend() const {
			return const_iterator();
		}

		iterator begin() const {
			if (head == nullptr) {
				return end();
			}
			std::stack<Node*> s;
			Node* min = minimum(head, s);

			return iterator(min, std::move(s));
		}
		iterator end() const {
			return iterator();
		}

		const_iterator find(const key_type& value) const {
			Node* current = head;
			std::stack<Node*> parents;
			while (current != nullptr) {

				if (comp(value, current->data)) {
					// we only keep track of items bigger than our data
					parents.push(current);
					current = current->left;
				}
				else if (comp(current->data, value)) {
					current = current->right;
				}
				else {
					return const_iterator(current, parents);
				}
			}
			return cend();
		}

		void clear() {
			if (head == nullptr) {
				return;
			}

			std::queue<Node*> q;
			Node** current;
			q.push(head);

			while (!q.empty()) {
				current = &(q.front());
				if ((*current)->left != nullptr) {
					q.push(((*current)->left));
				}
				if ((*current)->right != nullptr) {
					q.push(((*current)->right));
				}

				delete *current;
				*current = nullptr;
				current = nullptr;
				q.pop();
			}
			head = nullptr;
			size_ = 0;
		}

		void insert(const key_type& value) {

			if (find(value) != cend()) {
				return;
			}

			if (head == nullptr) {
				++size_;
				head = new Node(value);
			}

			bool need_rebalancing = false;
			std::stack<Node*> parents;
			Node* parent = nullptr;
			Node* current = head;
			bool quit = false;
			bool already_contains = false;

			while (!quit) {
				if (comp(value, current->data)) {
					parents.push(current);

					parent = current;
					current = current->left;
					if (current == nullptr) {
						++size_;
						parent->left = new Node(value);
						quit = true;
					}

				}
				else if (comp(current->data, value)) {
					parents.push(current);

					parent = current;
					current = current->right;
					if (current == nullptr) {
						++size_;
						parent->right = new Node(value);
						quit = true;
					}
				}
				else {
					quit = true; // the key already exists
					already_contains = true;
				}

			}
			if (!parents.empty() && !already_contains) {
				rebalance(parents);
			}
		}
	private:

		void rebalance(std::stack<Node*>& parents) {

			while (!parents.empty()) {

				Node* current = parents.top();
				current->height = get_height(current);
				int load_factor = get_factor(current);
				parents.pop();

				Node* parent = parents.empty() ? nullptr : parents.top();

				// node is doubly left heavy
				if (load_factor < -1) {
					int left_factor = get_factor(current->left);
					// left child is left heavy
					if (left_factor < 0) {
						current->height -= 2;

						right_rotate(current, parent);
					}
					// left child is balanced
					else if (left_factor == 0) {

						current->height--;
						current->left->height++;

						right_rotate(current, parent);
					}

					// left child is right heavy
					else {
						current->left->right->height++;
						current->left->height--;
						current->height -= 2;

						left_rotate((current)->left, current);
						right_rotate(current, parent);
					}
				}
				// node is doubly right heavy
				else if (load_factor > 1) {

					int right_factor = get_factor(current->right);
					// right child is right heavy
					if (right_factor > 0) {

						current->height -= 2;
						left_rotate(current, parent);
					}
					// right child is balanced
					else if (right_factor == 0) {

						current->height--;
						current->right->height++;
						left_rotate(current, parent);

					}
					// right child is left heavy
					else {

						current->right->left->height++;
						current->right->height--;
						current->height -= 2;

						right_rotate((current)->right, current);
						left_rotate(current, parent);
					}
				}
			}
		}

	public:

		void erase(const key_type& key) {

			Node* parent = nullptr;
			Node* current = head;
			std::stack<Node*> parents;
			bool found = false;

			while (current != nullptr) {

				if (comp(key, current->data)) {

					parents.push(current);

					parent = current;
					current = current->left;
				}
				else if (comp(current->data, key)) {

					parents.push(current);

					parent = current;
					current = current->right;
				}
				// found the item to be deleted
				else {
					delete_found(current, parent, parents);
					found = true;
				}
			}

			if (!parents.empty() && found) {
				rebalance(parents);
			}
		}
	private:

		void delete_found(Node*& current, Node*& parent, std::stack<Node*>& parents) {

			--size_;
			// no children
			if (current->left == nullptr && current->right == nullptr) {
				// means current is head
				if (parent == nullptr) {
					delete current;
					head = nullptr;
					current = nullptr;
				}
				else {
					if (parent->left == current) {
						parent->left = nullptr;
					}
					else {
						parent->right = nullptr;
					}
				}
			}
			// has no left children
			else if (current->left == nullptr) {
				if (current == head) {
					head = current->right;

				}
				else {
					if (parent->left == current) {
						parent->left = current->right;
					}
					else {
						parent->right = current->right;
					}
				}
			}
			// has no right children
			else if (current->right == nullptr) {

				if (current == head) {
					head = current->left;

				}
				else {
					if (parent->left == current) {
						parent->left = current->left;
					}
					else {
						parent->right = current->left;
					}
				}
			}
			// has both children
			else {

				std::queue<Node*> min_parents;
				Node* min = unlink_right_min(current, min_parents);
				replace_current_with_min(parent, current, min);

				// this workaround is needed because we want to insert
				// the minimum of the right subtree in the stack first as it will replace current
				parents.push(min);
				while (!min_parents.empty()) {
					parents.push(min_parents.front());
					min_parents.pop();
				}
			}
			delete current;
			current = nullptr;
		}

		// link min to parent instead of current
		void replace_current_with_min(Node*& parent, Node* child, Node*& new_child) {
			if (parent != nullptr) {
				if (parent->left == child) {
					parent->left = new_child;
				}
				else {
					parent->right = new_child;
				}
			}
			else {
				head = new_child;
			}

			new_child->right = child->right;
			new_child->left = child->left;
		}

		// unlinks and returns the minimum of the right subtree relative to Node* top
		Node* unlink_right_min(Node* const top, std::queue<Node*>& parents) {

			Node* min_parent = top;
			Node* min = top->right;

			while (min->left != nullptr) {

				min_parent = min;
				parents.push(min_parent);

				min = min->left;
			}

			if (min_parent->left == min) {
				min_parent->left = min->right;
			}
			else {
				min_parent->right = min->right;
			}
			return min;
		}

		Node* minimum(Node* current, std::stack<Node*>& stack) const {

			while (current->left != nullptr) {
				stack.push(current);
				current = current->left;
			}
			return current;
		}
		void right_rotate(Node* currNode, Node* parent) {

			Node* temp = currNode->left;
			currNode->left = temp->right;
			temp->right = currNode;

			if (parent != nullptr) {
				if (parent->left == currNode) {
					parent->left = temp;
				}
				else {
					parent->right = temp;
				}
			}
			else {
				head = temp;
			}
		}

		// performs a left rotation
		void left_rotate(Node* currNode, Node* parent) {

			Node* temp = currNode->right;
			currNode->right = temp->left;
			temp->left = currNode;

			if (parent != nullptr) {
				if (parent->left == currNode) {
					parent->left = temp;
				}
				else {
					parent->right = temp;
				}
			}
			else {
				head = temp;
			}
		}

		int get_height(Node* current) const {
			if (current == nullptr) {
				return -1;
			}
			int leftH = -1;
			int rightH = -1;
			if (current->left != nullptr) {
				leftH = current->left->height;
			}
			if (current->right != nullptr) {
				rightH = current->right->height;
			}

			return 1 + std::max(leftH, rightH);
		}

		int get_factor(Node* current) const {
			return get_height(current->right) - get_height(current->left);
		}

	};
}

#endif