#ifndef QUEUE_H
#define QUEUE_H

template <typename key_type> 
class queue {
private:
	struct Node {
		Node* next;
		key_type data;
		Node(const key_type& d) : data(d), next(nullptr) {}
		Node(key_type&& d) : data(std::move(d)), next(nullptr) {}
		Node() = delete;

	};
public:
	queue() : head(nullptr), size_(0) {}
	queue(const std::initializer_list<key_type>& list) {
		for (auto elem : list) {
			push(elem);
		}
	}
	queue& operator=(const queue<key_type>& rhs) {
		this->clear();

		Node* it = rhs.head;
		head = new Node(it->data);
		it = it->next;

		Node* prev = head;
		while (it != nullptr) {
			prev->next = new Node(it->data);
			prev = prev->next;
			last = it;
			it = it->next;

		}
		size_ = rhs.size_;
		return *this;
	}
	queue& operator=(queue<key_type>&& rhs) {
		this->clear();
		std::swap(this->head, rhs.head);
		std::swap(this->last, rhs.last);
		std::swap(size_, rhs.size_);
		return *this;
	}
	queue(const queue<key_type>& oth) {
		this->operator=(oth);
	}
	queue(queue<key_type>&& oth) : head(nullptr), last(nullptr), size_(0) {
		this->operator=(std::move(oth));
	}

	~queue() {
		clear();
	}
	void push(const key_type& value) {
		if (head == nullptr) {
			head = new Node(value);
			last = head;
		}
		else {
			last->next = new Node(value);
			last = last->next;
			if (size_ == 1) {
				head->next = last;
			}
		}
		++size_;
	}
	void push(key_type&& value) {
		if (head == nullptr) {
			head = new Node(std::move(value));
			last = head;
			back_parent = nullptr;
		}
		else {
			last->next = new Node(std::move(value));
			last = last->next;
		}
		++size_;
	}

	void pop() {
		if (size_ == 0) {
			throw std::exception("error popping queue; the queue was empty");
		}
		
		Node* new_head = head->next;
		delete head;
		head = new_head;
		--size_;
		if (size_ == 0) {
			last = nullptr;
		}
	}
	key_type& front() const {
		return head->data;
	}
	key_type& back() const {
		return last->data;
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
		last = nullptr;
	}

	Node* last;
	Node* head;
	size_t size_;
};

#endif