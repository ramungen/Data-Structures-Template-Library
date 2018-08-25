#pragma once
#include <string>
#include <utility>
#include <algorithm>
namespace data_structures {

	/*
	TODO:
	1. Crete Map and Set that inherit from BinaryTre
	*/

	template <typename key_type, class compare = std::less<key_type> >
	class AVL_tree {
	

	private:

		struct Node {

			key_type data;
			Node* left;
			Node* right;
			// - is left heavy, + is right heavy, 0 is neutral
			short load_factor;

			Node(const key_type& value) :
				data(value),
				right(nullptr),
				left(nullptr),
			    load_factor(0) {}
			Node(key_type&& value) :
				data(std::move(value)),
				right(nullptr),
				left(nullptr),
				load_factor(0) {}
		};

	private:

		template<typename key_type>
		class forward_iterator {
			// to access private constructors
			friend AVL_tree;

			using iterator_category = std::forward_iterator_tag;
			using pointer = Node*;
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
				parents= rhs.parents;
				return *this;
			}

		private:
			forward_iterator(pointer c, std::stack<pointer> p) : current(c), parents(p) {}
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
			key_type& operator*() const {
				if (current == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return current->data;
			}
			key_type* operator->() const {
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
		using tree_iterator = forward_iterator<key_type>;

	private:
		compare comp;
		size_t size_;
		Node * head;

	public:
		AVL_tree() : head(nullptr), size_(0) {}
		AVL_tree(const std::initializer_list<key_type>& list) {
			for (auto elem : list) {
				insert(elem);
			}
		}
		AVL_tree(std::initializer_list<key_type>&& list) {
			for (auto elem : list) {
				insert(elem);
			}
		}
		~AVL_tree() {
			this->clear();
		}

		bool empty() {
			return head == nullptr;
		}
		size_t size() {
			return size_;
		}

		tree_iterator begin() {
			if (head == nullptr) {
				return end();
			}
			std::stack<Node*> s;
			Node* min = minimum(head, s);
			
			return tree_iterator(min, std::move(s));
		}
		tree_iterator end() {
			return tree_iterator();
		}

		tree_iterator find(const key_type& value) {
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
					return tree_iterator(current, parents);
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

		void insert(const key_type& value) {

			if (find(value) != end()) {
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

			while (!quit) {
				if (value < current->data) {

					parents.push(current);
					--current->load_factor;

					parent = current;
					current = current->left;
					if (current == nullptr) {
						++size_;
						parent->left = new Node(value);
						quit = true;
					}

				}
				else if (value > current->data) {

					parents.push(current);
					++current->load_factor;
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
				if (parent != nullptr) {

					if (parent->load_factor > 1 || parent->load_factor < -1) {
						need_rebalancing = true;
					}

				}
				
			}
			if (need_rebalancing && !parents.empty()) {
				//rebalance(parents);
			}
		}
		private:

			void rebalance(std::stack<Node*>& parents) {
				int total_compensation = 0;
				while (!parents.empty()) {

					Node* current = parents.top();
					// oldCurrent is needed for adjusting the parent's load factor after a rotation
					Node* old_current = current;
					parents.pop();

					
					Node* parent;
					parent = parents.empty() ? nullptr : parents.top();
					// used to determine how to adjust the parents height after a rotation
					Node* old_parent_left = (parent == nullptr) ? nullptr : parent->left;
					// node is doubly left heavy
					if ((current)->load_factor < -1) {

						// left child is left heavy
						if ((current)->left->load_factor < 0) {
							++total_compensation;
							
							current->load_factor = 0;
							current->left->load_factor = 0;

							RightRotate(current, parent);
						}
						// left child is balanced
						else if ((current)->left->load_factor == 0) {

							current->load_factor = -1;
							current->left->load_factor = 1;

							RightRotate(current, parent);
						}

						// left child is right heavy
						else {
							
							++total_compensation;
							current->load_factor = 0;
							current->left->load_factor = 0;

							LeftRotate((current)->left, current);
							RightRotate(current, parent);
						}
					}
					// node is doubly right heavy
					else if ((current)->load_factor > 1) {

						// right child is right heavy
						if (current->right->load_factor > 0) {
							++total_compensation;
							
							current->load_factor = 0;
							current->right->load_factor = 0;

							LeftRotate(current, parent);
						}
						// right child is balanced
						else if (current->right->load_factor == 0) {
							current->load_factor = 1;
							current->right->load_factor = -1;

							LeftRotate(current, parent);

						}
						// right child is left heavy
						else {

							++total_compensation;
							current->load_factor = 0;
							current->right->load_factor = 0;

							RightRotate((current)->right, current);
							LeftRotate(current, parent);
						}
					}

					if (parent != nullptr) {
						// using old values for comparison because current ones change when rotating
						// but we need the old values in order to determine its parent's load factor
						if (old_parent_left == old_current) {
							parent->load_factor += total_compensation;
						}
						else {
							parent->load_factor -= total_compensation;
						}
					}
				}
			}

		public:

			void erase(const key_type& value) {

				// we use this to make it easier to track load factors
				if (find(value) == end()) {
					return;
				}

				Node* parent = nullptr;
				Node* current = head;
				std::stack<Node*> parents;
				bool need_rebalancing = false;

				while (current != nullptr) {

					if (value < current->data) {

						++current->load_factor;
						parents.push(current);

						parent = current;
						current = current->left;
					}
					else if (value > current->data) {

						--current->load_factor;
						parents.push(current);

						parent = current;
						current = current->right;
					}
					// found the item to be deleted
					else {
						delete_found(current, parent, parents);
					}

					if (parent != nullptr) {
						if (std::abs(parent->load_factor) > 1) {
							need_rebalancing = true;
						}


						if (std::abs(parent->load_factor) > 2) {
							throw std::exception("rebalancing in delete error; load factors not working			properly");
						}
					}
				}

				if (!parents.empty() && need_rebalancing) {
					//rebalance(parents);
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
				auto[minParent, min] = rightMin(current, parents);
				// TODO: change later to prevent copying
				current->data = min->data;
				eraseRightMinimum(minParent, min);
				return;
			}
		}
		
		void eraseRightMinimum(Node*& parent, Node*& current) {
			if (parent->left == current) {
				parent->left = current->left;
			}
			else {
				parent->right = current->left;
			}
			delete current;
			current = nullptr;
		}

		//// erases the minimum of the right subrtee
		//void eraseRightMinimum(Node** parent, Node** current) {
		//		if ((*parent)->left == (*current)) {
		//			(*parent)->left = (*current)->left;
		//		}
		//		else {
		//			(*parent)->right = (*current)->left;
		//		}
		//		delete *current;
		//		*current = nullptr;
		//}

		// finds the minimum of a right subtree and it's parent
		std::pair<Node*, Node*> rightMin(Node* node, std::stack<Node*>& parents) {

			Node* current = node->right;
			Node* parent = node;

			parent->load_factor--;
			parents.push(parent);

			while (current->left != nullptr) {

				parent = current;
				parent->load_factor++;
				parents.push(parent);

				current = current->left;
			}
			return std::make_pair(parent, current);
		}

		Node* minimum(Node* current, std::stack<Node*>& stack) {

			while (current->left != nullptr) {
				stack.push(current);
				current = current->left;
			}
			return current;
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

		// performs a left rotation
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

		public:
			// test functions
			int max_height_test() {
				return maxH(head);
			}	
			private:
				int maxH(Node* current) {
					if (current == nullptr) {
						return -1;
					}
					return 1 + std::max(maxH(current->left), maxH(current->right));
				}

		

	};
}

