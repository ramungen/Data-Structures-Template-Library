#pragma once
#include <string>
#include <utility>
#include <algorithm>
namespace DataStructures {

	/*
	TODO:
	1. Make it an AVL Tree
	2. Crete Map and Set that inherit from BinaryTree
	3. Add iterator support
	*/

	template <typename val_type>
	class BinaryTree {
	

	private:

		struct Node {
			val_type data;
			Node* left;
			Node* right;
			Node(const val_type& value) :
				data(value),
				right(nullptr),
				left(nullptr) {}
			Node(val_type&& value) :
				data(std::move(value)),
				right(nullptr),
				left(nullptr) {}
		};

	private:

		template<typename val_type>
		class forward_iterator {
			// to access private constructors
			friend BinaryTree;

			using iterator_category = std::forward_iterator_tag;
			using pointer = Node*;
			using reference = Node & ;
			using value = val_type;

		public:

			forward_iterator() : current(nullptr) {}
			forward_iterator(const forward_iterator* source) :
				current(source->current), 
				parents(source->parents) {}

			forward_iterator(const forward_iterator& source) : 
				current(source.current), 
				parents(source.parents) {}

			forward_iterator& operator=(const forward_iterator& rhs) {
				current = rhs.current;
				return *this;
			}

		private:
			forward_iterator(pointer c, std::stack<pointer>&& p) : current(c), parents(std::move(p)) {}
		public:

			forward_iterator& operator++() {
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
				// current being nullptr is hanled in operator++()
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
			val_type& operator*() const {
				if (current == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return current->data;
			}
			val_type* operator->() const {
				if (current == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return &current->data;
			}

		private:
			pointer current;
			std::stack<pointer> parents;
		};

	public:
		using iterator = forward_iterator<val_type>;

		size_t size_;
	private:
		Node * head;
	public:
		BinaryTree() : head(nullptr), size_(0) {
		}

		~BinaryTree() {
			this->clear();
		}

		bool empty() {
			return head == nullptr;
		}
		size_t size() {
			return size_;
		}

		iterator begin() {
			if (head == nullptr) {
				return end();
			}
			std::stack<Node*> s;
			Node* min = minimum(head, s);
			
			return iterator(min, std::move(s));
		}
		iterator end() {
			return iterator();
		}

		iterator find(const val_type& value) {
			Node* current = head;
			std::stack<Node*> parents;
			while (current != nullptr) {
				//parents.push(current);
				if (value < current->data){
					// we only keep track of items bigger than our data
					parents.push(current);
					current = current->left;
				}
				else if (value > current->data) {
					current = current->right;
				}
				else {
					return iterator(current, std::move(parents));
				}
			}
			return end();
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

		void insert(const val_type& value) {
			if (head == nullptr) {
				++size_;
				head = new Node(value);
			}
			Node* parent = nullptr;
			Node* current = head;
			bool quit = false;
			while (!quit) {
				if (value < current->data) {
					parent = current;
					current = current->left;
					if (current == nullptr) {
						++size_;
						parent->left = new Node(value);
						quit = true;
					}

				}
				else if (value > current->data) {
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
				}
			}
		}
		void erase(const val_type& value) {

			Node* parent = nullptr;
			Node* current = head;
			while (current != nullptr) {

				if (value < current->data) {
					parent = current;
					current = current->left;
				}
				else if (value > current->data) {
					parent = current;
					current = current->right;
				}
				// found the item to be deleted
				else {
					--size_;
					// the item has no children
					if (current->left == nullptr && current->right == nullptr) {
						// means current is head
						if (parent == nullptr) {
							delete current;
							head = nullptr;
						}
						else {
							if (parent->left == current) {
								parent->left = nullptr;
							}
							else {
								parent->right = nullptr;
							}

							delete current;
							current = nullptr;
						}
						
						return;
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
						delete current;
						current = nullptr;
						return;
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

						delete current;
						current = nullptr;
						return;
					}
					// has both children
					else {
						auto[minParent, min] = rightMin(current);
						// TODO: change later to prevent copying
						current->data = min->data;
						eraseRightMinimum(&minParent, &min);
						return;
					}
				}

			}
		}

	private:
		Node* minimum(Node* current, std::stack<Node*>& stack) {
			
			while (current->left != nullptr) {
				stack.push(current);
				current = current->left;
			}
			return current;
		}

		// erases the minimum of the right subrtee
		void eraseRightMinimum(Node** parent, Node** current) {
				if ((*parent)->left == (*current)) {
					(*parent)->left = (*current)->left;
				}
				else {
					(*parent)->right = (*current)->left;
				}
				delete *current;
				*current = nullptr;
		}

		// finds the minimum of a right subtree and it's parent
		std::pair<Node*, Node*> rightMin(Node* node) {
			Node* current = node->right;
			Node* parent = node;
			while (current->left != nullptr) {
				parent = current;
				current = current->left;
			}
			return std::make_pair(parent, current);
		}

		void RightRotate(Node* currNode, Node* parent) {
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

		public:
			void right_rotate_test() {
				RightRotate(head, nullptr);
			}
			void left_rotate_test() {
				LeftRotate(head, nullptr);
			}
			private:
		// performs a left rotation (seems to work so far)
		void LeftRotate(Node* currNode, Node* parent) {

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

	};
}

