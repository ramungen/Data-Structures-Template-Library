
#ifndef DLINKED_LIST
#define DLINKED_LIST

namespace dsl {
	// doubly linked list
	template<class val_type>
	class dlinked_list {


	private:
		struct Node;

		template<typename val_type>
		class bidirectional_iterator {

			using iterator_category = std::forward_iterator_tag;
			using value_type = Node;
			using pointer = Node* ;
			using reference = Node& ;

		private:

			// for accessing private methods
			friend class dlinked_list;

			bidirectional_iterator(pointer p) : ptr(p) {}

		public:

			bidirectional_iterator() : ptr(nullptr) {}
			bidirectional_iterator(const bidirectional_iterator& oth) : ptr(oth.ptr) {}
			bidirectional_iterator& operator=(const bidirectional_iterator& rhs) {

				ptr = rhs.ptr;
				return *this;
			}
			bidirectional_iterator& operator--() {
				if (ptr == nullptr) {
					throw std::exception("error advancing end iterator");
				}
				ptr = ptr->prev;
				return *this;
			}

			bidirectional_iterator& operator--(int dum) {
				bidirectional_iterator temp(*this);
				this->operator--();
				return temp;
			}

			bidirectional_iterator& operator++() {
				if (ptr == nullptr) {
					throw std::exception("error advancing end iterator");
				}
				ptr = ptr->next;
				return *this;
			}
			bidirectional_iterator operator++(int dum) {
				bidirectional_iterator temp(*this);
				this->operator++();
				return temp;
			}
			bool operator ==(const bidirectional_iterator& rhs) const {
				return ptr == rhs.ptr;
			}
			bool operator !=(const bidirectional_iterator& rhs) const {
				return ptr != rhs.ptr;
			}
			val_type& operator*() const {
				if (ptr == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return ptr->data;
			}
			val_type* operator->() const {
				if (ptr == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return &ptr->data;
			}

		private:
			pointer ptr;

		};

	public:
		
		using iterator = bidirectional_iterator<val_type>;
		using const_iterator = bidirectional_iterator<const val_type>;

		dlinked_list() :
			head(nullptr),
			tail(nullptr),
			size_(0) {}

		dlinked_list(const dlinked_list<val_type>& rhs) {
			Node* iter = rhs.head;
			while (iter != nullptr) {
				push_back(iter->data);
				iter = iter->next;

			}

		}

		iterator begin() const {
			return iterator(head);
		}

		iterator end() const {
			return iterator();
		}

		const_iterator cbegin() const {
			return const_iterator(head);
		}

		const_iterator cend() const {
			return const_iterator();
		}

		dlinked_list(const std::initializer_list<val_type>& list) {
			for (auto elem : list) {
				push_back(elem);
			}
		}

		void erase(iterator& where) {
			Node* current = where.ptr;
			if (current->next != nullptr) {
				current->next->prev = current->prev;
			}
			if (current->prev != nullptr) {
				current->prev->next = current->next;
			}

			where.ptr = current->prev;
			delete current;

		}

		iterator insert(iterator where, const val_type& value) {
			Node* current = where.ptr;
			
			if (where == end()) {
				push_back(value);
				return iterator(tail);
			}
			else if (where == begin()) {
				push_front(value);
				return iterator(head);
			}
			Node* newNode = new Node(value);

			current->prev->next = newNode;
			newNode->next = current;

			return iterator(newNode);
		}

		dlinked_list(dlinked_list<val_type>&& rhs) : head(nullptr), tail(nullptr), size_(0) {
			std::swap(this->head, rhs.head);
			std::swap(this->tail, rhs.tail);
			std::swap(this->size_, rhs.size_);
		}

		~dlinked_list() {
			clear();

		}
		void clear() {
			Node* iter = head;
			Node* next = iter;
			while (iter) {
				next = iter->next;
				delete iter;
				iter = next;
			}
			head = nullptr;
			tail = nullptr;
			size_ = 0;
		}

		dlinked_list<val_type>& operator=(dlinked_list<val_type>&& rhs) {
			clear();
			std::swap(head, rhs.head);
			std::swap(tail, rhs.tail);
			std::swap(size_, rhs.size_);
			return *this;
		}


		dlinked_list<val_type>& operator=(const dlinked_list<val_type>& rhs) {
			Node* iter = rhs.head;
			while (iter) {
				push_back(iter->data);
				iter = iter->next;

			}
			return *this;
		}


		void append(dlinked_list<val_type>&& other) {

			this->tail->next = other.head;
			other.head->prev = tail;
			if (other.head != nullptr) {
				other.head->prev = this->tail;
				other.head->prev = this->tail;
			}
			size_ += other.size();
		}

		void push_front(const val_type& value) {
			Node* newNode = new Node(value);
			if (head == nullptr) {
				head = newNode;
				head->next = nullptr;
				head->prev = nullptr;
				tail = head;

			}
			else {
				newNode->next = head;
				newNode->prev = nullptr;
				head->prev = newNode;
				head = newNode;
				if (head->next == nullptr) {
					tail = head;
				}
			}
			++size_;

		}

		void push_back(val_type const& value) {
			if (head == nullptr) {
				push_front(value);
			}
			else {
				Node* newNode = new Node(value);
				tail->next = newNode;
				newNode->prev = tail;
				tail = newNode;
				tail->next = nullptr;
				++size_;
			}
		}


		void erase(val_type const& value) {
			if (head->data == value) {
				pop_front();
			}
			else if (tail->data == value) {
				pop_back();

			}
			else {
				Node* iter = head->next;
				while (iter) {
					if (iter->data == value) {
						if (iter->prev) {
							iter->prev->next = iter->next;
						}
						if (iter->next) {
							iter->next->prev = iter->prev;
						}
						delete iter;
						iter = nullptr;
						--size_;
						return;
					}
					iter = iter->next;
				}
				throw "there is no such value";
			}
		}

		void pop_front() {
			if (head->next == nullptr && head->prev == nullptr) {
				delete head;
				head = nullptr;
			}
			else {
				head = head->next;
				delete head->prev;
				head->prev = nullptr;
			}
			--size_;
		}

		void pop_back() {
			if (tail->prev == nullptr) {
				delete head;
				head = nullptr;
				tail = nullptr;
			}
			else {
				tail = tail->prev;
				delete tail->next;
				tail->next = nullptr;
			}
			--size_;
		}

		size_t size() const {
			return size_;
		}


	private:
		struct Node {
			Node(const val_type& val) : data(val), next(nullptr), prev(nullptr) {}

			void link(Node* newNext) {
				this->next = newNext;
				newNext->prev = next;

			}

			Node* next;
			Node* prev;
			val_type data;


		};
		Node* head;
		Node* tail;
		size_t size_;
	};

}
#endif
