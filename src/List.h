
#ifndef LIST_H
#define LIST_H

#include <iostream>

template<class T>
class List {
public:
	List();
	void pushFront(T const& value);
	void pushBack(T const& value);
	void remove(T const& value);
	void removeFront();
	void removeBack();

	void print();
	void printReverse();

private:
	struct Node {
		T data;
		Node* next;
		Node* prev;
	};
	Node* head;
	Node* tail;
};



template<class T>
List<T>::List() :
	head(nullptr),
	tail(nullptr)
{}

template<class T>
void List<T>::pushFront(T const& value) {

	Node* newNode = new Node;
	newNode->data = value;
	newNode->next = head;
	head = newNode;
	if (head->next == nullptr) {
		tail = head;
	}

}
template <class T>
void List<T>::pushBack(T const& value) {
	if (head == nullptr) {
		pushFront(value);
	}
	else {
		Node* newNode = new Node;
		newNode->data = value;
		tail->next = newNode;
		tail = newNode;
		tail->next = nullptr;
	}
}
template<class T>
void List<T>::remove(T const& value) {
	if (head->data == value) {
		removeFront();
	}
	else if (tail->data == value) {
		removeBack();

	}
	else {
		Node* iter = head->next;
		while (iter != nullptr) {
			if (iter->data == value) {
				if (iter->prev != nullptr) {
					iter->prev->next = iter->next;
				}
				if (iter->next != nullptr) {
					iter->next->prev = iter->prev;
				}
				delete iter;
				iter = nullptr;
				return;
			}
			iter = iter->next;
		}
		throw "there is no such value";
	}
}
template<class T>
void List<T>::removeFront() {
	if (head->next == nullptr && head->prev == nullptr) {
		delete head;
		head = nullptr;
	}
	else {
		head = head->next;
		delete head->prev;
	}
}
template <class T>
void List<T>::removeBack() {
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
}

template<class T>
void List<T>::print() {
	Node* iter = head;
	while (iter != nullptr) {
		std::cout << iter->data << "->";
		iter = iter->next;
	}
	std::cout << "NULL\n";
}

template <class T>
void List<T>::printReverse() {
	Node* iter = tail;
	while (iter != nullptr) {
		std::cout << iter->data << ' ';

		iter = iter->prev;
	}
	std::cout << std::endl;
}



#endif
