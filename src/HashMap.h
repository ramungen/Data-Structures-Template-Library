#ifndef HASHMAP_H
#define HASHMAP_H


#include <functional>
#include <random>
#include <utility>
#include <vector>
#include <iostream>
#include <initializer_list>



/*
THINGS TO DO:
1. use inheritance to remove code similarities betweeh HashMap and HashSet

*/

template<typename key_type, typename val_type, 
	typename prehash = std::hash<key_type> >

	class HashMap {

	private:

		typedef typename std::pair<const key_type, val_type> pair;



		struct Node {
			Node(pair&& elem) :
			data(std::move(elem)),
			next(nullptr)
			{

			}
			Node(const key_type& first, const val_type& second) :
				data(first, second),
				next(nullptr)
			{

			}
			Node* next;
			pair data;
		};


		class bucket_iterator :
			public std::iterator<std::forward_iterator_tag, Node > {

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
			pair& operator*() const {
				if (ptr == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return ptr->data;
			}
			pair* operator->() const {
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


			bucket_iterator find(const val_type& value) {

				Node* iter = head;
				while (iter) {
					if (iter->data.first == value) {
						return bucket_iterator(iter);
					}
					iter = iter->next;
				}
				bucket_iterator it(iter);
				return it;
			}
			bool empty() {
				return list_length == 0;
			}

			Bucket() :
				list_length(0), head(nullptr)
			{}

			void push_front(pair&& value) {

				Node* temp = new Node(std::move(value));
				temp->next = nullptr;
				temp->next = head;
				head = temp;
				++list_length;
			}

			val_type& create_if_not_found(const key_type& key) {
				if (head == nullptr) {
					val_type new_value;
					if (std::is_arithmetic<val_type>::value == true) { // custom constructs only arithmetic types
						new_value = 0;
					}
					Node* new_node = new Node(key, new_value);
					head = new_node;
					head->next = nullptr;
					++list_length;
					return head->data.second;
				}
				Node* iter = head;
				while (iter->next != nullptr) {
					if (iter->data.first == key) {
						return iter->data.second;
					}
				}
				val_type new_value;
				if (std::is_arithmetic<val_type>::value == true) { // custom constructs only arithmetic types
					new_value = 0;
				}

				Node* new_node = new Node(key, new_value);
				iter->next = new_node;
				++list_length;
				return new_node->data.second;
			}


			void push_front(const pair& value) {

				Node* temp = new Node(value.first, value.second);
				temp->next = nullptr;
				temp->next = head;
				head = temp;
				++list_length;
			}

			bool erase(const val_type& value) {

				if (head == nullptr) {
					return false;
				}
				Node* iter = head;
				if (head->data.first == value) {
					head = head->next;
				}
				else {
					Node* prev = nullptr;
					while (iter->data.first != value) {
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


			unsigned int size() {
				return list_length;
			}

			void printMap() const {
				Node* iter = head;
				while (iter != nullptr) {
					std::cout << iter->data.first << '(' << iter->data.second << ')' << " --> ";
					iter = iter->next;
				}
				std::cout << "NULL\n";
			}

		private:

			Node* head;
			unsigned int list_length;

		};


		std::vector< Bucket > map;
		unsigned int elements;
		static const unsigned int starting_size;
		prehash prehasher;
		int rand1, rand2;

		int start_pos;
		int end_pos;

		float load_factor; 




	private:

		class forward_iterator :
			public std::iterator<std::forward_iterator_tag, Bucket> {

			friend class HashMap; // to access private constructors
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
			forward_iterator(pointer p, bucket_iterator liter, HashMap* prnt, long long int pos_) : ptr(p),
				ptr_it(liter),
				parent(prnt),
				pos(pos_)
			{}
			forward_iterator(reference p, bucket_iterator liter, HashMap* prnt, long long int pos_) :
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
					while (parent->map[pos].empty()) {
						++pos;
						if (pos > parent->end_pos) {
							ptr = nullptr;
							ptr_it = ptr->end();
							return *this;
						}
					}
					ptr = &(parent->map[pos]);
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
			std::pair<const key_type, val_type>& operator*() const { // change
				if (ptr == nullptr || ptr_it == ptr->end()) {
					throw std::exception("error dereferencing an invalid iterator");
				}

				return *ptr_it;
			}
			std::pair<const key_type, val_type>* operator->() const { // change
				if (ptr == nullptr || ptr_it == ptr->end()) {
					throw std::exception("error dereferencing an invalid iterator");
				}

				return &(*ptr_it);
			}

		private:

			pointer ptr;
			bucket_iterator ptr_it;
			HashMap* parent;
			long long int pos;
		};



	public:

		typedef bucket_iterator local_iterator;
		typedef typename HashMap<key_type, val_type>::forward_iterator iterator;



		HashMap(float factor = 0.75f) :
			load_factor(factor),
			elements(0),
			start_pos(INT_MAX),
			end_pos(-1),
			map(starting_size) {

			if (load_factor >= 1) {
				load_factor = 0.75f; // default value
			}

			std::random_device rd;
			std::mt19937 generator(rd());
			std::uniform_int_distribution<int> U(20, 10000);
			rand1 = U(generator);
			rand2 = U(generator);
		}

		HashMap(const std::initializer_list<std::pair<key_type, val_type> >& list)
			: HashMap()

		{
			for (const auto& elem : list) {
				insert(elem);
			}
		}

		HashMap(HashMap& oth) :
			elements(oth.elements),
			prehasher(oth.prehasher),
			rand1(oth.rand1),
			rand2(oth.rand2),
			start_pos(oth.start_pos),
			end_pos(oth.end_pos),
			map(oth.map.size())
		{

			for (auto& elem : oth) {

				long long int hash_key = hash(map.size(), elem.first);
				map[hash_key].push_front(elem);
			}

		}
		HashMap(HashMap&& oth) :
			elements(0),
			rand1(0),
			rand2(0),
			start_pos(0),
			end_pos(0),
			load_factor(0)
		{
			map.swap(oth.map);
			std::swap(load_factor, oth.load_factor);
			std::swap(elements, oth.elements);
			std::swap(prehasher, oth.prehasher);
			std::swap(rand1, oth.rand1);
			std::swap(rand2, oth.rand2);
			std::swap(start_pos, oth.start_pos);
			std::swap(end_pos, oth.end_pos);

		}

		HashMap& operator=( HashMap& rhs) { // needs optimizing

			load_factor = rhs.load_factor;
			prehasher = rhs.prehasher;
			rand1 = rhs.rand1;
			rand2 = rhs.rand2;
			elements = rhs.elements;
			start_pos = rhs.start_pos;
			end_pos = rhs.end_pos;
			clear();
			map.resize(rhs.map.size());
			for (auto& elem : rhs) {

				long long int hash_key = hash(map.size(), elem.first);
				map[hash_key].push_front(elem);
			}

			return *this;
		}
		HashMap& operator=(HashMap&& rhs) { 
			clear();
			map.swap(rhs.map);
			std::swap(elements, rhs.elements);
			prehasher = rhs.prehasher;
			rand1 = rhs.rand1;
			rand2 = rhs.rand2;
			std::swap(start_pos, rhs.start_pos);
			std::swap(end_pos, rhs.end_pos);
			return *this;
		}

		~HashMap() {

		}

		iterator begin() {
			if (empty()) {
				return end();
			}
			iterator it(&map[start_pos], map[start_pos].begin(), this, start_pos);
			return it;
		}
		iterator end() {
			return iterator();
		}




		iterator find(const val_type& key) {
			
			long long int hash_key = hash(map.size(), key);

			bucket_iterator list_iter = map[hash_key].find(key);

			if (list_iter == map[hash_key].end()) {
				return end();
			}
			else {
				return iterator(&map[hash_key], list_iter, this, hash_key);
			}
		}

		val_type& at(const key_type& key) {
			
			long long int hash_key = hash(map.size(), key);

			bucket_iterator list_iter = map[hash_key].find(key);

			if (list_iter == map[hash_key].end()) {
				throw std::exception("element not found in the container");
			}
			return (*list_iter).second;
		}
		void insert(pair&& elem) {
			long long int hash_key = hash(map.size(), elem.first);

			if (map[hash_key].find(elem.first) == map[hash_key].end()) {
				update_start_and_end(hash_key);

				map[hash_key].push_front(std::forward<pair>(elem));
				++elements;
				grow();
			}
		}
		void insert(const pair& elem) {

			long long int hash_key = hash(map.size(), elem.first);

			if (map[hash_key].find(elem.first) == map[hash_key].end()) {
				update_start_and_end(hash_key);

				map[hash_key].push_front(elem);
				++elements;
				grow();
			}

		}

		void insert(std::initializer_list<std::pair<key_type, val_type> >& list) {
			for (auto& elem : list) {
				insert(elem);
			}
		}

		size_t count(const key_type& key) {
			
			return (find(key) != end()) ? 1 : 0;
		}

		val_type& operator[](const key_type& key) {

			long long int hash_key = hash(map.size(), key);

			return map[hash_key].create_if_not_found(key);

		}


		void erase(const key_type& key) {
			if (empty()) {
				return;
			}

			long long int hash_key = hash(map.size(), key);

			if (map[hash_key].erase(key)) {

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


		void rehash(unsigned int n) {
			if (n <= elements) {
				return;
			}
			else if (n < map.size() && load_factor * 0.8 >= (float)elements / n) {
				
				start_pos = 2147483647;
				end_pos = -1;

				std::vector<Bucket> temp(n);
				for (auto& bucket : map) {
					for (auto& element : bucket) {

						//long long int prehashKey = prehasher(element.first);
						long long int hash_key = hash(temp.size(), element.first);

						update_start_and_end(hash_key);

						temp[key].push_front(element);
					}
				}

				map = std::move(temp);
			}
			else {

				start_pos = INT_MAX;
				end_pos = -1;
				int possibility = (int)((float)map.size() * 1.15f);
				if (possibility > n) {
					n = possibility;
				}
				std::vector< Bucket > temp(n);
				for (auto& bucket : map) {
					for (auto& element : bucket) {

						long long int key = hash(temp.size(), element.first);

						update_start_and_end(key);

						temp[key].push_front(element);
					}
				}
				map = std::move(temp);
			}
		}
			void clear() {
				std::vector< Bucket > new_map(starting_size);
				map = std::move(new_map);
				elements = 0;
				start_pos = INT_MAX;
				end_pos = -1;
			}

			void reserve(unsigned int n) {
				if (n < starting_size) {
					return;
				}
				unsigned int new_size = (unsigned int)((float)(n + 1) / load_factor);
				rehash(new_size);
			}


			unsigned int buckets() {
				return (unsigned int)map.size();
			}


			void traverse() {
				for (const auto& bucket : map) {
					bucket.printMap();
				}
			}

			int size() {
				return elements;
			}

			bool empty() {
				return elements == 0;
			}

			private:

				void update_start_and_end(long long int hash_key) {
					if (start_pos > hash_key) {
						start_pos = (int)hash_key;
					}
					if (end_pos < hash_key) {
						end_pos = (int)hash_key;
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
						if (map[++start_pos].empty() == false) {
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
						if (map[--end_pos].empty() == false) {
							return;
						}
					}
				}

				long long int hash(size_t size, const key_type& key) {

					long long int prehash_key = prehasher(key);
					int prime = 8188057;
					
					long long int new_key = (rand1 * prehash_key + rand2);
					new_key %= prime;
					new_key %= size;
					return new_key;
				}

				void shrink() { 

					if (map.size() > starting_size && (float)map.size() * load_factor >= elements * 4) {

						start_pos = 2147483647;
						end_pos = -1;

						std::vector<Bucket> temp((int)((float)map.size() * load_factor / 2));
						for (auto& bucket : map) {			
							for (auto& element : bucket) {  

								long long int hash_key = hash(temp.size(), element.first);

								update_start_and_end(hash_key);

								temp[hash_key].push_front(element);
							}
						}
	
						map = std::move(temp);
					}
				}


				void grow() { 

					if ((float)elements / map.size() >= load_factor) {

						start_pos = 2147483647;
						end_pos = -1;

						std::vector<Bucket > temp(2 * map.size());
						for (auto& bucket : map) {			
							for (auto& element : bucket) {  

								long long int hash_key = hash(temp.size(), element.first);

								update_start_and_end(hash_key);


								temp[hash_key].push_front(element);
							}
						}
						map = std::move(temp);

					}
				}

	};

	template<typename key_type, typename val_type,
		typename prehash = std::hash<key_type> >
	const unsigned int HashMap<key_type, val_type, prehash>::starting_size = 20;

#endif
