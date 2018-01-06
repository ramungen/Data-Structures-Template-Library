
#ifndef LIST_H
#define LIST_H

#include <iostream>


namespace dat {

	template<class T>
	class DLinkedList {
	public:
		DLinkedList();
		DLinkedList(const DLinkedList<T>& rhs);
		DLinkedList(DLinkedList<T>&& rhs);
		~DLinkedList();
		DLinkedList<T>& operator=(DLinkedList<T>&& rhs);
		DLinkedList<T>& operator=(const DLinkedList<T>& rhs);
		bool empty() const { return head == nullptr; } 
		void append(DLinkedList<T>&& other);
		void push_front(T const& value);
		void push_back(T const& value);
		void erase(T const& value);
		void pop_front();
		void pop_back();
		unsigned int length() const { return listLength; }
		void print() const;
		void print_reverse() const;

	private:
		struct Node {
			Node* next;
			Node* prev;
			T data;
		};
		Node* head;
		Node* tail;
		unsigned int listLength;
	};



	template<class T>
	DLinkedList<T>::DLinkedList() :
		head(nullptr),
		tail(nullptr),
		listLength(0)
	{}

	template<class T>
	DLinkedList<T>::DLinkedList(const DLinkedList<T>& rhs) {
		Node* iter = rhs.head;
		while (iter) {
			push_back(iter->data);
			iter = iter->next;
		    
		}
		
	}

	template<class T>
	DLinkedList<T>::DLinkedList(DLinkedList<T>&& rhs) : head(nullptr), tail(nullptr), listLength(0) {
		std::swap(this->head, rhs.head);
		std::swap(this->tail, rhs.tail);
		std::swap(this->listLength, rhs.listLength);
	}

	template<class T>
	DLinkedList<T>::~DLinkedList() {
		Node* iter = head;
		Node* next = iter;
		while (iter) {
			next = iter->next;
			delete iter;
			iter = next;
		}
	}

	template<class T>
	DLinkedList<T>& DLinkedList<T>::operator=(DLinkedList<T>&& rhs) {
		head = nullptr;
		tail = nullptr;
		listLength = 0;
		std::swap(head, rhs.head);
		std::swap(tail, rhs.tail);
		std::swap(listLength, rhs.listLength);
		return *this;
	}

	template<class T>
	DLinkedList<T>& DLinkedList<T>::operator=(const DLinkedList<T>& rhs) {
		Node* iter = rhs.head;
		while (iter) {
			push_back(iter->data);
			iter = iter->next;

		}
		return *this;
	}

	template<class T>
	void DLinkedList<T>::append(DLinkedList<T>&& other) {
		this->tail->next = other.head;
		if (other.head) {
			other.head.prev = this->tail;
		}
		this->ListLength += other.length();
	}

	template<class T>
	void DLinkedList<T>::push_front(T const& value) {
		Node* newNode = new Node;
		newNode->data = value;
		newNode->prev = nullptr;
		newNode->next = nullptr;
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
		++listLength;

	}
	template <class T>
	void DLinkedList<T>::push_back(T const& value) {
		if (head == nullptr) {
			push_front(value);
		}
		else {
			Node* newNode = new Node;
			
			newNode->data = value;
			tail->next = newNode;
			newNode->prev = tail;
			tail = newNode;
			tail->next = nullptr;
			++listLength;
		}
	}

	template<class T>
	void DLinkedList<T>::erase(T const& value) {
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
					--listLength;
					return;
				}
				iter = iter->next;
			}
			throw "there is no such value";
		}
	}
	template<class T>
	void DLinkedList<T>::pop_front() {
		if (head->next == nullptr && head->prev == nullptr) {
			delete head;
			head = nullptr;
		}
		else {
			head = head->next;
			delete head->prev;
			head->prev = nullptr;
		}
		--listLength;
	}
	template <class T>
	void DLinkedList<T>::pop_back() {
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
		--listLength;
	}

	template<class T>
	void DLinkedList<T>::print() const {
		Node* iter = head;
		while (iter) {
			std::cout << iter->data << "->";
			iter = iter->next;
		}
		std::cout << "NULL\n";
	}

	template <class T>
	void DLinkedList<T>::print_reverse() const {
		Node* iter = tail;
		while (iter) {
			std::cout << iter->data << ' ';

			iter = iter->prev;
		}
		std::cout << '\n';
	}


}
#endif
