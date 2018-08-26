
#ifndef DLINKED_LIST
#define DLINKED_LIST

#include <utility>

namespace data_structures {
	// doubly linked list
	template<class val_type>
	class dlinked_list {
	public:

		dlinked_list() :
			head(nullptr),
			tail(nullptr),
			list_length(0) {}

		dlinked_list(const dlinked_list<val_type>& rhs) {
			Node* iter = rhs.head;
			while (iter) {
				push_back(iter->data);
				iter = iter->next;

			}

		}

		dlinked_list(const std::initializer_list<val_type>& list) {
			for (auto elem : list) {
				insert(elem);
			}
		}

		dlinked_list(dlinked_list<val_type>&& rhs) : head(nullptr), tail(nullptr), list_length(0) {
			std::swap(this->head, rhs.head);
			std::swap(this->tail, rhs.tail);
			std::swap(this->list_length, rhs.list_length);
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
			list_length = 0;
		}

		dlinked_list<val_type>& operator=(dlinked_list<val_type>&& rhs) {
			clear();
			std::swap(head, rhs.head);
			std::swap(tail, rhs.tail);
			std::swap(list_length, rhs.list_length);
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
			if (other.head) {
				other.head.prev = this->tail;
			}
			this->ListLength += other.length();
		}


		void push_front(val_type const& value) {
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
			++list_length;

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
				++list_length;
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
						--list_length;
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
			--list_length;
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
			--list_length;
		}

	private:
		struct Node {
			Node(const val_type& val) : data(val), next(nullptr), prev(nullptr) {}

			Node* next;
			Node* prev;
			val_type data;


		};
		Node* head;
		Node* tail;
		unsigned int list_length;
	};

}
#endif
