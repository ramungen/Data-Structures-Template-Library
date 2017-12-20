#pragma once

#include <cstdlib>
#include <iostream>

template <typename T>
class LinkedList {

public:

	void LinkedList<T>::reverse();
	void pushFront(T value);
	void pushBack(T value);
	void remove(T value);
	bool find(T value);
	LinkedList merge(LinkedList second);
	void sort();
	void insertAt(unsigned int at, T value);
	LinkedList operator+(LinkedList other);
	LinkedList();
	~LinkedList();
	void print();
	unsigned int length();


private:

	struct Node {
		int data;
		Node* next;
	};

	Node* head;
	Node* prev;
	unsigned int listLength;
};


template <typename T>
void LinkedList<T>::pushFront(T value) {
	Node* temp = new Node;
	Node* second;
	temp->data = value;
	temp->next = nullptr;
	if (head == nullptr) {
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
void LinkedList<T>::reverse() {
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
void LinkedList<T>::pushBack(T value) {
	Node* temp = new Node;
	temp->data = value;
	temp->next = nullptr;
	if (head == nullptr) {
		head = temp;
	}
	else {
		Node* iter = head;
		while (iter->next != nullptr) {
			iter = iter->next;
		}
		iter->next = temp;
	}
	++listLength;
}
template <typename T>
void LinkedList<T>::remove(T value) {
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
bool LinkedList<T>::find(T value) {

	Node* iter = head;
	while (iter != nullptr) {
		if (iter->data == value) {
			return true;
		}
		iter = iter->next;
	}
	return false;
}

template <typename T>
LinkedList<T> LinkedList<T>::merge(LinkedList& second) {
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
void LinkedList<T>::insertAt(unsigned int at, T value) {

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
LinkedList<T> LinkedList<T>::operator+(LinkedList<T> other) {
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
LinkedList<T>::LinkedList() {
	listLength = 0;
	head = nullptr;
}
template <typename T>
LinkedList<T>::~LinkedList() {
	head = nullptr;
}

template <typename T>
unsigned int LinkedList<T>::length() {
	return listLength;
}

template <typename T>
void LinkedList<T>::print() {
	Node* iter = head;
	while (iter != nullptr) {
		std::cout << iter->data << ' ';
		iter = iter->next;
	}

}

template <typename T>
void LinkedList<T>::sort() {
	Node* current = head;
	Node* next = nullptr;
	while (current != nullptr && current->next != nullptr) { // insertion sort
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