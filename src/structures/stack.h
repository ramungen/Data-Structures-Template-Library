#ifndef STACK_H
#define STACK_H
#include <exception>
namespace dsl {


	template <typename key_type>
	class stack {
	private:
		struct Node {
			Node* next;
			key_type data;
			Node(const key_type& d) : data(d), next(nullptr) {}
			Node(key_type&& d) : data(std::move(d)), next(nullptr) {}
			Node() : next(nullptr) {}

		};

		Node* head;
		size_t size_;

	public:
		stack() : head(nullptr), size_(0) {}
		stack(const std::initializer_list<key_type>& list) {
			for (auto elem : list) {
				push(elem);
			}
		}
		stack(const stack<key_type>& oth) {
			this->clear();
			size_ = 0;
			this->operator=(oth);
		}
		stack(stack<key_type>&& oth) : head(nullptr), size_(0) {
			this->operator=(std::move(oth));
		}
		stack& operator=(const stack<key_type>& rhs) {
			this->clear();
			if (rhs.head == nullptr) {
				return *this;
			}
			Node* it = rhs.head;
			head = new Node(it->data);

			Node* prev = head;
			while (it != nullptr) {
				prev->next = new Node(it->data);
				prev = prev->next;
				it = it->next;
			}
			size_ = rhs.size();
			return *this;
		}
		stack& operator=(stack&& rhs) {
			this->clear();
			std::swap(this->head, rhs.head);
			std::swap(this->size_, rhs.size_);
			return *this;
		}

		~stack() {
			clear();
		}
		void push(const key_type& value) {
			if (head == nullptr) {
				head = new Node(value);
			}
			else {
				Node* new_head = new Node(value);
				new_head->next = head;
				head = new_head;
			}
			++size_;
		}
		void push(key_type&& value) {
			if (head == nullptr) {
				head = new Node(std::move(value));
			}
			else {
				Node* new_head = new Node(std::move(value));
				new_head->next = head;
				head = new_head;
			}
			++size_;
		}

		void pop() {
			if (size_ == 0) {
				throw std::exception("error popping stack; the stack was empty");
			}

			Node* new_head = head->next;
			delete head;
			head = new_head;

			--size_;
		}
		key_type& top() const {
			return head->data;
		}
		size_t size() const {
			return size_;
		}
		bool empty() const {
			return size_ == 0;
		}
	private:
		void clear() {
			Node* it = head;
			Node* temp = nullptr;
			while (it != nullptr) {
				temp = it;
				it = it->next;
				delete temp;
				temp = nullptr;
			}
			head = nullptr;
		}

	};

}

#endif