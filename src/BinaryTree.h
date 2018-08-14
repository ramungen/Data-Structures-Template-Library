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
			Node(val_type value) :
				data(value),
				right(nullptr),
				left(nullptr) {}
		};

	public:
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
			if (head == nullptr) {
				return;
			}
			Node* parent = head;
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
				else {
					--size_;

					if (current->left == nullptr && current->right == nullptr) {
						if (current == head) {
							head = nullptr;
						}
							if (parent->left == current) {
								parent->left = nullptr;
							}
							else {
								parent->right = nullptr;
							}
						
						delete current;
						current = nullptr;
						return;
					}
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
					else {
						auto[minParent, min] = rightMin(current);
						// change later to prevent copying
						current->data = min->data;
						eraseRightMinimum(&minParent, &min);
						return;
					}
				}

			}
		}

	private:

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

	};
}

