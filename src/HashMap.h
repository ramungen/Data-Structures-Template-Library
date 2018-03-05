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
 1. implement iterator support
 2. replace contains() with find()
 3. add the same functionality as in set()
 4. make pair have std::pair member variable instead of first/second, so more efficient
 5. make key non-modifiable and value modifiable when accessing them via iterators
 6. implement load factor
 7. as of now const_iterator is non existant, need to make it
*/

/*
Things to implement after those above:
1. Rvalue reference support
2. operator[]

*/

template<typename key_type, typename val_type, 
	typename prehash = std::hash<key_type> >

	class HashMap {

	private:

		typedef typename std::pair<key_type, val_type> pair;


		struct Node {

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
			//friend  class bucket_iterator;
			
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
				return listLength == 0;
			}

			Bucket() :
				listLength(0), head(nullptr)
			{}

			void push_front(const pair&& value) {
				Node* temp = new Node;
				temp->data = value;
				temp->next = nullptr;
				temp->next = head;
				head = temp;
				++listLength;
			}


			void push_front(const pair& value) {

				Node* temp = new Node;
				temp->data = value;
				temp->next = nullptr;
				temp->next = head;
				head = temp;
				++listLength;
			}

			bool erase(const val_type& value) {


				if (head == nullptr) {
					return false;
				}
				Node* iter = head;
				if (head->data.first == value) {

					if (head->next != nullptr) {
						head = head->next;
					}
					else {

						delete head;
						head = nullptr;
					}
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
				--listLength;
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
				return listLength;
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
			unsigned int listLength;

		};


			std::vector< Bucket > map;
			unsigned int elements;
			static const unsigned int starting_size;
			prehash prehasher;
			int rand1, rand2;

			int start_pos; 
			int end_pos;

			float load_factor; // not implemented yet




	private:

		template<typename key_type, typename val_type>
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
					if (pos >= parent->map.size()) {
						ptr = nullptr;
						ptr_it = ptr->end();
						return *this;
					}
					while (parent->map[pos].empty()) {
						++pos;
						if (pos >= parent->map.size()) {
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
			std::pair<key_type, val_type>& operator*() const { // change
				if (ptr == nullptr || ptr_it == ptr->end()) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				
				return *ptr_it;
			}
			std::pair<key_type, val_type>* operator->() const { // change
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
		typedef forward_iterator<val_type, key_type> iterator;
		typedef forward_iterator<val_type, const key_type> const_iterator;

		public:


			HashMap() :
				elements(0),
				start_pos(INT_MAX),
				end_pos(-1),
				map(starting_size) {

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

			bool contains(const key_type& value) { // deprecated
				long long int prehashKey = prehasher(value);
				long long int trueKey = hash(prehashKey, map.size());
	
				return map[trueKey].find(p);
			}

			void insert(const pair& elem) { 
				long long int prehashKey = prehasher(elem.first);
				long long int trueKey = hash(prehashKey, map.size());

				if (map[trueKey].find(elem.first) == map[trueKey].end()) {
					update_start_and_end(trueKey);

					map[trueKey].push_front(elem);
					++elements;
					grow();
				}

			}


			void erase(const val_type& value) { // needs reworking
				if (empty()) {
					return;
				}

				long long int prehashKey = prehasher(value);
				long long int trueKey = hash(prehashKey, map.size());
				
				if (map[trueKey].erase(value)) {
					--elements;

					if (start_pos == trueKey) {
						update_start_pos();
					}
					if (end_pos == trueKey) {
						update_end_pos();
					}

					shrink();
				}
			}

			public:

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

				void update_start_and_end(long long int trueKey) {
					if (start_pos > trueKey) {
						start_pos = trueKey;
					}
					if (end_pos < trueKey) {
						end_pos = trueKey;
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

				long long int hash(long long int prehashKey, size_t size) {
					long long int oldKey = prehashKey;
					int prime = 8188057;

					long long int trueKey = (rand1 * oldKey + rand2);
					trueKey %= prime;
					trueKey %= size;
					return trueKey;
				}

				void shrink() { // need to implement load factor
					// function might be broken (erase() does not work)
					start_pos = 2147483647;
					end_pos = -1;

					if (map.size() > starting_size && map.size() >= elements * 4) {
						std::vector<Bucket> temp(map.size() / 2);
						for (auto& bucket : map) {			// const auto& does not work as of now
							for (auto& element : bucket) {  // const auto& does not work as of now

								long long int prehashKey = prehasher(element.first);
								long long int trueKey = hash(prehashKey, (unsigned int)temp.size());

								update_start_and_end(trueKey);

								temp[trueKey].push_front(element);
							}
						}
						map = std::move(temp);
					}
				}


				void grow() { // need to implement load factor
					if (map.size() > elements) {
						return;
					}

					start_pos = 2147483647;
					end_pos = -1;

					std::vector<Bucket > temp(2 * map.size());
					for (auto& bucket : map) {			// const auto& does not work as of now
						for (auto& element : bucket) {  // const auto& does not work as of now

							long long int prehashKey = prehasher(element.first);
							long long int trueKey = hash(prehashKey, (unsigned int)temp.size());

							update_start_and_end(trueKey);


							temp[trueKey].push_front(element);
						}
					}
					map = std::move(temp);

				}

	};

	template<typename key_type, typename val_type,
		typename prehash = std::hash<key_type> >
	const unsigned int HashMap<key_type, val_type, prehash>::starting_size = 20;

#endif
