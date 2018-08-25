
#ifndef HASHSET_H
#define HASHSET_H

#include <vector>
#include <algorithm>
#include <random>
#include <initializer_list>




/*
	THINGS TO DO:
	1. remove code similarities with HashMap via inheritance/composition
	2. implement const_iterator
*/
template <typename key_type, 
	typename prehash = std::hash<key_type> >

	class HashSet {
	
	private:

		struct Node {

			Node* next;
			key_type data;
		};
		class bucket_iterator {

			using iterator_category = std::forward_iterator_tag;
			using pointer = Node*;
			using reference = Node&;

		public:

			bucket_iterator() : ptr(nullptr) {}
			bucket_iterator(pointer p) : ptr(p) {}
			bucket_iterator(reference p) : ptr(&p) {}
			bucket_iterator(const bucket_iterator* source) : ptr(source->ptr) {}
			bucket_iterator& operator=(const bucket_iterator& rhs) {

				ptr = rhs.ptr;
				return *this;
			}
			bucket_iterator& operator++() {
				if (ptr == nullptr) {
					throw std::exception("error advancing end iterator");
				}
				ptr = ptr->next;
				return *this;
			}
			bucket_iterator operator++(int dum) {
				if (ptr == nullptr) {
					throw std::exception("error advancing end iterator");
				}
				forward_iterator temp(this);
				ptr = ptr->next;
				return temp;
			}
			bool operator ==(const bucket_iterator& rhs) const {
				return ptr == rhs.ptr;
			}
			bool operator !=(const bucket_iterator& rhs) const {
				return ptr != rhs.ptr;
			}
			key_type& operator*() const {
				if (ptr == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return ptr->data;
			}
			key_type* operator->() const {
				if (ptr == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return &ptr->data;
			}

		private:
			pointer ptr;

		};

		class Bucket {

		public:

			bucket_iterator begin() {
				return bucket_iterator(head);
			}

			bucket_iterator end() {
				return bucket_iterator();
			}

			bool empty() {
				return list_length == 0;
			}
			bucket_iterator find(const key_type& value) {

				Node* iter = head;
				while (iter) {
					if (iter->data == value) {
						return bucket_iterator(iter);
					}
					iter = iter->next;
				}
				bucket_iterator it(iter);
				return it;
			}

			Bucket() :
				list_length(0), head(nullptr)
			{}

			void push_front(key_type&& value) {
				Node* temp = new Node;
				temp->data = value;
				temp->next = nullptr;
				temp->next = head;
				head = temp;
				++list_length;
			}


			void push_front(const key_type& value) {

				Node* temp = new Node;
				temp->data = value;
				temp->next = nullptr;
				temp->next = head;
				head = temp;
				++list_length;
			}

		
			bool erase(const key_type& value) {
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
				--list_length;
				delete iter;
				return true;
			}


			~Bucket() {
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
				return list_length;
			}


			void print() const {
				Node* iter = head;
				while (iter != nullptr) {
					std::cout << iter->data << " --> ";
					iter = iter->next;
				}
				std::cout << "NULL\n";
			}

		private:

			Node* head;
			unsigned int list_length;
		};




	private:


		
		std::vector< Bucket > set;
		unsigned int elements;
		static const unsigned int starting_size;
		prehash prehasher;
		int rand1, rand2;

		int start_pos;
		int end_pos;

		float load_factor;
	

	private:

		template<typename key_type>
		class forward_iterator {

			using iterator_category = std::forward_iterator_tag;
			using pointer = Bucket*;
			using reference = Bucket&;

			friend class HashSet; // to access private constructors
		public:
			forward_iterator() : 
				ptr(nullptr), 
				parent(nullptr),
				pos(-1) 
			{}

			forward_iterator(const forward_iterator* rhs) :
				ptr(rhs->ptr),
				parent(rhs->parent),
				ptr_it(rhs->ptr_it),
				pos(rhs->pos)
			{}
			forward_iterator(const forward_iterator& rhs) :
				ptr(rhs.ptr),
				parent(rhs.parent),
				ptr_it(rhs.ptr_it),
				pos(rhs.pos)
			{}

		private:
			forward_iterator(pointer p, bucket_iterator liter, HashSet* prnt, long long int pos_) : ptr(p),
				ptr_it(liter),
				parent(prnt),
				pos(pos_)
			{}
			forward_iterator(reference p, bucket_iterator liter, HashSet* prnt, long long int pos_) :
				ptr(p),
				ptr_it(liter),
				parent(prnt),
				pos(pos_)
			{}

		
		public:
			

			forward_iterator& operator=(forward_iterator rhs) {
				ptr = rhs.ptr;
				ptr_it = rhs.ptr_it;
				parent = rhs.parent;
				pos = rhs.pos;
				return *this;
			}
			forward_iterator operator++() {
				if (ptr == nullptr) {
					throw std::exception("error advancing end iterator");
				}

				if (ptr_it != ptr->end()) {
					++(ptr_it);
				}
				if (ptr_it == ptr->end()) {
					++pos;
					if (pos > parent->end_pos) {
						ptr = nullptr;
						ptr_it = ptr->end();
						return *this;
					}
					while (parent->set[pos].empty()) {
						++pos;
						if (pos > parent->end_pos) {
							ptr = nullptr;
							ptr_it = ptr->end();
							return *this;
						}
					}
					ptr = &(parent->set[pos]);
					ptr_it = ptr->begin();
				}

				return *this;
			}

			forward_iterator operator++(int) {
				if (ptr == nullptr) {
					throw std::exception("error advancing end iterator");
				}
				forward_iterator temp(this);
				operator++();
				return temp;
			}
			bool operator ==(const forward_iterator rhs) const {
				return (ptr == rhs.ptr) && (ptr_it == rhs.ptr_it);
			}
			bool operator !=(const forward_iterator rhs) const {
				return !(*this == rhs);
			}
			const key_type& operator*() const {
				if (ptr == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return *ptr_it;
			}
			const key_type& operator->() const {
				if (ptr == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return &(*ptr_it);
			}

		private:
			pointer ptr;
			bucket_iterator ptr_it;
			HashSet* parent;
			long long int pos;
		};


	public:

		typedef forward_iterator<key_type> iterator;
		typedef forward_iterator<const key_type> const_iterator;

		HashSet(float factor = 0.75f) :
			load_factor(factor),
			elements(0),
			start_pos(INT_MAX),
			end_pos(-1),
			set(starting_size) {

			if (load_factor >= 1) {
				load_factor = 0.75f; // default value
			}

			std::random_device rd;
			std::mt19937 generator(rd());
			std::uniform_int_distribution<int> U(20, 10000);
			rand1 = U(generator);
			rand2 = U(generator);
		}

		HashSet(const std::initializer_list<key_type>& list)
			: HashSet()

		{
			for (const auto& elem : list) {
				insert(elem);
			}
		}

		HashSet(HashSet& oth) :
			elements(oth.elements),
			prehasher(oth.prehasher),
			rand1(oth.rand1),
			rand2(oth.rand2),
			start_pos(oth.start_pos),
			end_pos(oth.end_pos),
			set(oth.set.size())
		{

			for (auto& elem : oth) {

				long long int hash_key = hash(elem, set.size());
				set[hash_key].push_front(elem);
			}

		}
		HashSet(HashSet&& oth) :
			elements(0),
			rand1(0),
			rand2(0),
			start_pos(0),
			end_pos(0),
			load_factor(0)
		{

			set.swap(oth.set);
			std::swap(load_factor, oth.load_factor);
			std::swap(elements, oth.elements);
			std::swap(prehasher, oth.prehasher);
			std::swap(rand1, oth.rand1);
			std::swap(rand2, oth.rand2);
			std::swap(start_pos, oth.start_pos);
			std::swap(end_pos, oth.end_pos);

		}

		HashSet& operator=(HashSet& rhs) { // needs optimizing

			clear();
			load_factor = rhs.load_factor;
			prehasher = rhs.prehasher;
			rand1 = rhs.rand1;
			rand2 = rhs.rand2;
			elements = rhs.elements;
			start_pos = rhs.start_pos;
			end_pos = rhs.end_pos;
			
			set.resize(rhs.set.size());

			for (auto elem : rhs) {

				long long int hash_key = hash(elem, set.size());
				set[hash_key].push_front(elem);
			}

			return *this;
		}
		HashSet& operator=(HashSet&& rhs) {
			clear();
			set.swap(rhs.set);
			std::swap(elements, rhs.elements);
			prehasher = rhs.prehasher;
			rand1 = rhs.rand1;
			rand2 = rhs.rand2;
			std::swap(start_pos, rhs.start_pos);
			std::swap(end_pos, rhs.end_pos);
			return *this;
		}

		iterator find(const key_type& value) { 
			long long int prehash_key = prehasher(value);
			long long int hash_key = hash(prehash_key, set.size());

			list_iterator list_iter = set[hash_key].find(value);
			
			if (list_iter == set[hash_key].end()) {
				return end();
			}
			else {
				iterator temp(&set[hash_key], list_iter, this, hash_key);
				return temp;
			}
		}

		iterator begin() {
			if (empty()) {
				return end();
			}
			iterator it(&set[start_pos], set[start_pos].begin(), this, start_pos);
			return it;
		}

		iterator end() {
			return iterator();
		}

		void insert(const key_type& value) { 
			long long int prehash_key = prehasher(value);
			long long int hash_key = hash(prehash_key, set.size());

			if (set[hash_key].find(value) == set[0].end()) {

				update_start_and_end(hash_key);

				set[hash_key].push_front(value);
				++elements;
				grow();
			}
		}
		void insert(const key_type&& value) { 
			long long int prehash_key = prehasher(value);
			long long int hash_key = hash(prehash_key, set.size());

			if (set[hash_key].find(value) == set[hash_key].end()) {

				update_start_and_end(hash_key);
				set[hash_key].push_front(std::move(value));
				++elements;
				grow();
			}
		}
		void insert(const std::initializer_list<key_type>& list) {
			for (const auto& element : list) {
				insert(element);
			}
		}
		void insert(const std::initializer_list<key_type>&& list) {
			for (const auto& element : list) {
				insert(std::move(element));
			}
		}
		

		void erase(const key_type& value) { 
			if (empty()) {
				return;
			}

			long long int prehash_key = prehasher(value);
			long long int hash_key = hash(prehash_key, set.size());
			if (set[hash_key].erase(value)) {
				--elements;
				if (start_pos == hash_key) {
					update_start_pos();
				}
				if (end_pos == hash_key) {
					update_end_pos();
				}
				shrink();
			}
		}

		unsigned int buckets() {
			return (unsigned int)set.size();
		}


		void traverse() {
			for (const auto& bucket : set) {
				bucket.print();
			}
		}

		int size() {
			return elements;
		}

		bool empty() {
			return elements == 0;
		}
		void rehash(unsigned int n) {
			if (n <= set.size()) {
				return;
			}
			start_pos = INT_MAX;
			end_pos = -1;
			int possibility = (int)((float)set.size() * 1.15f);
			if (possibility > n) {
				n = possibility;
			}
			std::vector<Bucket > (n);
			for (const auto& bucket : set) {
				for (const auto& element : bucket) {

					long long int prehash_key = prehasher(element);
					long long int hash_key = hash(prehash_key, (unsigned int)temp.size());

					update_start_and_end(hash_key);

					temp[hash_key].push_front(element);
				}
			}
			set = std::move(temp);
		}
		void clear() {
			std::vector<Bucket> new_set(starting_size);
			set = std::move(new_set);
			elements = 0;
			start_pos = INT_MAX;
			end_pos = -1;
		}

		void reserve(unsigned int n) {
			if (n < (float)starting_size *load_factor) {
				return;
			}
			unsigned int new_size = (unsigned int)((float)(n+1) / load_factor);
			rehash(new_size);
		}


	private:

		void update_start_and_end(long long int hash_key) {
			if (start_pos > (int)hash_key) {
				start_pos = (int)hash_key;
			}
			if (end_pos < hash_key) {
				end_pos = hash_key;
			}
		}

		void update_start_pos() {
			if (empty()) {
				start_pos = 2147483647;
				return;
			}
			if (elements == 1) {
				start_pos = end_pos;
				return;
			}
			while (start_pos < end_pos) {
				if (set[++start_pos].empty() == false) {
					return;
				}
			}
		}
		void update_end_pos() {
			if (empty()) {
				end_pos = -1;
				return;
			}
			if (elements == 1) {
				end_pos = start_pos;
				return;
			}
			while (end_pos > start_pos) {
				if (set[--end_pos].empty() == false) {
					return;
				}
			}
		}

		


		long long int hash(long long int prehash_key, size_t size) {
			long long int oldKey = prehash_key;
			int prime = 8188057;

			long long int hash_key = (rand1 * oldKey + rand2);
			hash_key %= prime;
			hash_key %= size;
			return hash_key;
		}

		void shrink()  { 
			if (set.size() > starting_size && (float)set.size() * load_factor >= elements * 4) {
				start_pos = 2147483647;
				end_pos = -1;
				std::vector<Bucket> temp(set.size() / 2);
				for (auto& bucket : set) {
					for (auto& element : bucket) {

						long long int prehash_key = prehasher(element);
						long long int hash_key = hash(prehash_key, (unsigned int)temp.size());

						update_start_and_end(hash_key);

						temp[hash_key].push_front(element);
					}
				}
				set = std::move(temp);
			}
		}


		void grow() { 
		
		
			if((float)elements / set.size() >= load_factor) {

				start_pos = 2147483647;
				end_pos = -1;

				std::vector<Bucket > temp(2 * set.size());
				for (auto& bucket : set) {
					for (auto& element : bucket) {

						long long int prehash_key = prehasher(element);
						long long int hash_key = hash(prehash_key, (unsigned int)temp.size());

						update_start_and_end(hash_key);

						temp[hash_key].push_front(element);
					}
				}
				set = std::move(temp);
			}
		}
};

template<typename key_type, typename prehash = PreHash<key_type> >
const unsigned int HashSet<key_type, prehash>::starting_size = 20;

#endif
