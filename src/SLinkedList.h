#pragma once

#include <cstdlib>
#include <iostream>

namespace dat {

	template <typename T>
	class SLinkedList {

	public:
		SLinkedList(SLinkedList<T>&& rhs);
		SLinkedList(SLinkedList<T>& rhs);
		SLinkedList();
		~SLinkedList();

		void reverse();
		void push_front(T value);
		void push_back(T value);
		void erase(T value);
		bool find(T value);
		SLinkedList merge(SLinkedList& second);
		void sort();
		void inset_at(unsigned int at, T value);
		SLinkedList operator+(SLinkedList other);

		void print();
		unsigned int length();
		SLinkedList<T>& operator=(SLinkedList<T>&& rhs);
		SLinkedList<T>& operator=(SLinkedList<T>& rhs);

	private:

		struct Node {
			T data;
			Node* next;
		};
		Node* head;
		unsigned int listLength;
	};


	template <typename T>
	SLinkedList<T>::SLinkedList() :
		listLength(0), head(nullptr)
	{}


	template <typename T>
	void SLinkedList<T>::push_front(T value) {
		Node* temp = new Node;
		Node* second;
		temp->data = value;
		temp->next = nullptr;
		if (!head) {
			head = temp;
			head->next = nullptr;
		}
		else {
			second = head;
			head = temp;
			head->next = second;
		}
		++listLength;
	}

	template <class T>
	void SLinkedList<T>::reverse() {
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
	template <typename T>
	SLinkedList<T>::SLinkedList(SLinkedList<T>&& rhs) :
		head(nullptr), listLength(0)
	{
		std::swap(head, rhs.head);
		std::swap(listLength, rhs.listLength);
	}
	template <typename T>
	SLinkedList<T>::SLinkedList(SLinkedList<T>& rhs) :
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

	template <typename T>
	SLinkedList<T>& SLinkedList<T>::operator=(SLinkedList<T>&& rhs) {
		std::swap(head, rhs.head);
		std::swap(listLength, rhs.listLength);
		std::cout << "rvalue ref called!!!!\n";
		return *this;
	}

	template <typename T>
	SLinkedList<T>& SLinkedList<T>::operator=(SLinkedList<T>& rhs) {
		std::cout << " lvalue ref called\n";
		head = rhs.head;
		listLength = rhs.head;
		return *this;
	}


	template <typename T>
	void SLinkedList<T>::push_back(T value) {
		Node* temp = new Node;
		temp->data = value;
		temp->next = nullptr;
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
	template <typename T>
	void SLinkedList<T>::erase(T value) {
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
					std::cout << "there is no such element " << value << std::endl;
					return;
				}
				prev = iter;
				iter = iter->next;

			}
			prev->next = iter->next;
		}
		--listLength;
		delete iter;
	}
	template <typename T>
	bool SLinkedList<T>::find(T value) {

		Node* iter = head;
		while (iter) {
			if (iter->data == value) {
				return true;
			}
			iter = iter->next;
		}
		return false;
	}

	template <typename T>
	SLinkedList<T> SLinkedList<T>::merge(SLinkedList& second) {
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

	template <typename T>
	void SLinkedList<T>::inset_at(unsigned int at, T value) {

		Node* newNode = new Node;
		newNode->data = value;
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

	template <typename T>
	SLinkedList<T> SLinkedList<T>::operator+(SLinkedList<T> other) {
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


	template <typename T>
	SLinkedList<T>::~SLinkedList() {
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

	template <typename T>
	unsigned int SLinkedList<T>::length() {
		return listLength;
	}

	template <typename T>
	void SLinkedList<T>::print() {
		Node* iter = head;
		while (iter != nullptr) {
			std::cout << iter->data << ' ';
			iter = iter->next;
		}

	}

	template <typename T>
	void SLinkedList<T>::sort() {
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

}