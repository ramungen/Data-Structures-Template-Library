#ifndef SLINKED_LIST_H
#define SLINKED_LIST_H


#include <iostream>
#include <iterator>

template <typename val_type>
class slinked_list {

private:
	struct Node;

	template<typename val_type>
	class forward_iterator {

		using iterator_category = std::forward_iterator_tag;
		using value_type = Node;
		using pointer = Node*;
		using reference = Node&;

	public:

		forward_iterator() : ptr(nullptr) {}
		forward_iterator(pointer p) : ptr(p) {}
		forward_iterator(reference p) : ptr(&p) {}
		forward_iterator(const forward_iterator* source) : ptr(source->ptr) {}
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

	typedef forward_iterator<val_type> iterator;
	typedef forward_iterator<const val_type> const_iterator;

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
		return listLength == 0;
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
		listLength(0), head(nullptr)
	{}

	void push_front(const val_type&& value) {
		Node* temp = new Node(std::move(value));
		temp->next = head;
		head = temp;
		++listLength;
	}

	
	void push_front(const val_type& value) {

		Node* temp = new Node(value);
		temp->next = head;
		head = temp;
		++listLength;
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
		head(nullptr), listLength(0)
	{
		std::swap(head, rhs.head);
		std::swap(listLength, rhs.listLength);
	}
	
	slinked_list(slinked_list<val_type>& rhs) :
		listLength(rhs.listLength)
	{
		if (rhs.head) {
			Node* iter = rhs.head;
			for (int i = 0; i < rhs.length(); ++i) {
				this->push_back(iter->data);
				iter = iter->next;
			}
		}
	}
	
	slinked_list<val_type>& operator=(slinked_list<val_type>&& rhs) { 
		std::swap(head, rhs.head);
		std::swap(listLength, rhs.listLength);
		~rhs;
		return *this;
	}

	
	slinked_list<val_type>& operator=(slinked_list<val_type>& rhs) {
		head = rhs.head;
		listLength = rhs.head;
		return *this;
	}


	void push_back(val_type value) {
		Node* temp = new Node(value);
		last = temp;
		if (!head) {
			head = temp;
		}
		else {
			Node* iter = head;
			while (iter->next) {
				iter = iter->next;
			}
			iter->next = temp;
		}
		++listLength;
	}


	bool erase(val_type value) {
		if (head == nullptr) {
			return false;
		}
		Node* iter = head;
		if (head->data == value) {
			if (head->next != nullptr) {
				head = head->next;
			}
			else {
				head = nullptr;
			}
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
			prev->next = iter->next;
		}
		--listLength;
		delete iter;
		return true;
	}

	
	slinked_list<val_type> merge(slinked_list& second) {
		if (this->head == nullptr) {
			return second;
		}
		if (second.head == nullptr) {
			return *this;
		}
		Node* iter = head;

		while (iter->next != nullptr) {
			iter = iter->next;
		}
		iter->next = second.head;
		this->listLength += second.listLength;
		return *this;
	}

	
	void insert_at(unsigned int at, val_type value) {

		Node* newNode = new Node(value);
		if (at == 0) {
			if (head == nullptr) {
				head = newNode;
				head->next = nullptr;
				++listLength;
				return;
			}
			else {
				Node* temp = head;
				head = newNode;
				head->next = temp;
				++listLength;
				return;
			}
		}

		Node* iter = head;
		Node* prev = nullptr;

		int count = 0;

		while (iter != nullptr) {

			if (count == at) {

				prev->next = newNode;
				newNode->next = iter;
				return;
			}
			prev = iter;
			iter = iter->next;
			count++;
		}
		if (count == at) {
			prev->next = newNode;
			newNode->next = iter;
		}
		else {
			throw std::out_of_range("insertion place specified was out of bounds");
		}
	}

	
	slinked_list<val_type> operator+(slinked_list<val_type>& other) {
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
		Node* next = nullptr;
		if (head) {
			next = head->next;
			delete head;
		}
		Node* curr;
		while (next) {
			curr = next;
			next = curr->next;
			delete curr;
		}
	}

	
	unsigned int length() {
		return listLength;
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
	unsigned int listLength;
};


#endif