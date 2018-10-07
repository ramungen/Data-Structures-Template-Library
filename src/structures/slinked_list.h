#ifndef SLINKED_LIST_H
#define SLINKED_LIST_H


#include <iostream>
#include <iterator>

namespace dsl {
	template <typename val_type>
	class slinked_list {

	private:
		struct Node;

		template<typename val_type>
		class forward_iterator {

			using iterator_category = std::forward_iterator_tag;
			using value_type = Node;
			using pointer = Node* ;
			using reference = Node& ;

		private:

			// for accessing the private constructor
			friend class slinked_list;
			forward_iterator(pointer p) : ptr(p) {}

		public:

			forward_iterator() : ptr(nullptr) {}
			forward_iterator(const forward_iterator& source) : ptr(source->ptr) {}
			forward_iterator& operator=(const forward_iterator& rhs) {

				ptr = rhs.ptr;
				return *this;
			}
			forward_iterator& operator++() {
				if (ptr == nullptr) {
					throw std::exception("error advancing end iterator");
				}
				ptr = ptr->next;
				return *this;
			}
			forward_iterator operator++(int dum) {
				if (ptr == nullptr) {
					throw std::exception("error advancing end iterator");
				}
				forward_iterator temp(this);
				ptr = ptr->next;
				return temp;
			}
			bool operator ==(const forward_iterator& rhs) const {
				return ptr == rhs.ptr;
			}
			bool operator !=(const forward_iterator& rhs) const {
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

		using iterator = forward_iterator<val_type>;
		using const_iterator = forward_iterator<const val_type>;

		iterator begin() const {
			return iterator(head);
		}

		const_iterator cbegin() const {
			return const_iterator(head);
		}

		iterator end() const {
			return iterator();
		}
		const_iterator cend() const {
			Node* last = nullptr;
			return const_iterator(last);
		}

		bool empty() {
			return size_ == 0;
		}
		iterator find(const val_type& value) {

			Node* iter = head;
			while (iter) {
				if (iter->data == value) {
					return iterator(iter);
				}
				iter = iter->next;
			}
			iterator it(iter);
			return it;
		}

		slinked_list() :
			size_(0), head(nullptr) {}

		void push_front(const val_type&& value) {
			Node* temp = new Node(std::move(value));
			temp->next = head;
			head = temp;
			++size_;
		}


		void push_front(const val_type& value) {

			Node* temp = new Node(value);
			temp->next = head;
			head = temp;
			++size_;
		}

		void reverse() {
			Node* iter = head;
			Node* prev = nullptr;
			Node* next = head;
			while (iter->next != nullptr) {
				next = iter->next;
				iter->next = prev;
				prev = iter;
				iter = next;
			}
			iter->next = prev;
			head = iter;
		}

		slinked_list(slinked_list<val_type>&& rhs) :
			head(nullptr), size_(0) {
			std::swap(head, rhs.head);
			std::swap(size_, rhs.size_);
		}

		slinked_list(slinked_list<val_type>& rhs) :
			size_(rhs.size_) {
			if (rhs.head) {
				Node* iter = rhs.head;
				for (int i = 0; i < rhs.size(); ++i) {
					this->push_back(iter->data);
					iter = iter->next;
				}
			}
		}

		slinked_list<val_type>& operator=(slinked_list<val_type>&& rhs) {
			this->clear();
			
			std::swap(head, rhs.head);
			std::swap(size_, rhs.size_);
			return *this;
		}


		slinked_list<val_type>& operator=(const slinked_list<val_type>& rhs) {
			this->clear();

			for (auto& elem : rhs) {
				push_back(elem);
			}
			return *this;
		}


		void push_back(const val_type& value) {
			Node* temp = new Node(value);

			if (head == nullptr) {
				head = temp;
				last = temp;
			}
			else {
				Node* prevLast = last;
				prevLast->next = temp;
			}

			last = temp;
			++size_;
		}


		bool erase(const val_type& value) {
			if (head == nullptr) {
				return false;
			}
			Node* iter = head;
			if (head->data == value) {
				Node* temp = head->next;
				delete head;
				head = temp;
			}
			else {
				Node* prev = nullptr;
				while (iter->data != value) {
					if (iter->next == nullptr) {
						return false;
					}
					prev = iter;
					iter = iter->next;

				}
				if (iter == last) {
					last = prev;
				}
				prev->next = iter->next;
			}
			--size_;
			delete iter;
			return true;
		}


		void merge(slinked_list<val_type>&& second) {
			last->next = second.head;
			size_ += second.size_;
		}


		slinked_list<val_type>& operator+(slinked_list<val_type>& other) {
			if (head == nullptr) {
				return other;
			}
			Node* iter = head;
			while (iter->next != nullptr) {
				iter = iter->next;
			}
			iter->next = other.head;
			return *this;
		}

		~slinked_list() {
			clear();
		}


		unsigned int size() {
			return size_;
		}

		void clear() {
			Node* current = head;
			while (current != nullptr) {
				Node* temp = current;
				current = current->next;
				delete temp;
			}
			head = nullptr;
			size_ = 0;
		}

		void pop_front() {
			Node* temp = head;
			head = temp->next;
			delete temp;
		}


		void sort() {

			Node* current = head;
			Node* next = nullptr;
			while (current && current->next) { // insertion sort
				next = current->next;
				while (next != nullptr) {
					if (next->data < current->data) {
						std::swap(next->data, current->data);
					}
					next = next->next;
				}
				current = current->next;
			}
		}

	private:
		struct Node {
			Node(const val_type& val) : data(val), next(nullptr) {}
			Node* next;
			val_type data;
		};


	private:

		Node* head;
		Node* last;
		size_t size_;
	};

}

#endif