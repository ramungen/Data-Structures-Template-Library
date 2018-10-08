#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <algorithm>
#include <random>
#include <initializer_list>


namespace dsl::implementation {

	template <typename key_type,
		typename prehash = std::hash<key_type> , class Compare = std::less<key_type> >

		class hash_table {

		private:

			struct Node {
				Node() = delete;
				Node(const key_type& key) :
					data(key), next(nullptr) {}

				Node* next;
				key_type data;
			};

		private:

			class bucket_iterator {

				using iterator_category = std::forward_iterator_tag;
				using pointer = Node * ;
				using reference = Node & ;

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
						if (!comp(value, iter->data) && !comp(iter->data, value)) {
							return bucket_iterator(iter);
						}
						iter = iter->next;
					}
					bucket_iterator it(iter);
					return it;
				}

				Bucket() :
					list_length(0), head(nullptr) {}

				void push_front(key_type&& value) {
					Node* temp = new Node(std::move(value));
					temp->next = head;
					head = temp;
					++list_length;
				}


				void push_front(const key_type& value) {

					Node* temp = new Node(value);
					temp->next = head;
					head = temp;
					++list_length;
				}


				bool erase(const key_type& value) {
					if (head == nullptr) {
						return false;
					}
					Node* iter = head;
					// equal
					if (!comp(value, head->data) && !comp(head->data, value)) {
						if (head->next != nullptr) {
							head = head->next;
						}
						else {
							head = nullptr;
						}
					}
					else {
						Node* prev = nullptr;
						// not equal
						while (comp(value, iter->data) || comp(iter->data, value)) {
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

			private:

				Node * head;
				unsigned int list_length;
				Compare comp;
			};

		private:
			std::vector< Bucket > table;
			size_t elements;
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
				using pointer = Bucket * ;
				using reference = Bucket & ;
				using value = key_type;

				friend class hash_table; // to access private constructors
			public:
				forward_iterator() :
					ptr(nullptr),
					parent(nullptr),
					pos(-1) {}

				forward_iterator(const forward_iterator* rhs) :
					ptr(rhs->ptr),
					parent(rhs->parent),
					ptr_it(rhs->ptr_it),
					pos(rhs->pos) {}
				forward_iterator(const forward_iterator& rhs) :
					ptr(rhs.ptr),
					parent(rhs.parent),
					ptr_it(rhs.ptr_it),
					pos(rhs.pos) {}

			private:
				forward_iterator(pointer p, bucket_iterator liter, hash_table* prnt, long long int pos_) : ptr(p),
					ptr_it(liter),
					parent(prnt),
					pos(pos_) {}
				forward_iterator(reference p, bucket_iterator liter, hash_table* prnt, long long int pos_) :
					ptr(p),
					ptr_it(liter),
					parent(prnt),
					pos(pos_) {}


			public:


				forward_iterator & operator=(forward_iterator rhs) {
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
						while (parent->table[pos].empty()) {
							++pos;
							if (pos > parent->end_pos) {
								ptr = nullptr;
								ptr_it = ptr->end();
								return *this;
							}
						}
						ptr = &(parent->table[pos]);
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
				value& operator*() const { // change
					if (ptr == nullptr || ptr_it == ptr->end()) {
						throw std::exception("error dereferencing an invalid iterator");
					}

					return *ptr_it;
				}
				value* operator->() const { // change
					if (ptr == nullptr || ptr_it == ptr->end()) {
						throw std::exception("error dereferencing an invalid iterator");
					}

					return &(*ptr_it);
				}

			private:

				pointer ptr;
				bucket_iterator ptr_it;
				hash_table* parent;
				long long int pos;
			};


		public:

			using iterator = forward_iterator<key_type>;
			using const_iterator = forward_iterator<const key_type>;

			hash_table(float factor = 0.75f) :
				load_factor(factor),
				elements(0),
				start_pos(INT_MAX),
				end_pos(-1),
				table(starting_size) {

				if (load_factor >= 1) {
					load_factor = 0.75f; // default value
				}

				std::random_device rd;
				std::mt19937 generator(rd());
				std::uniform_int_distribution<int> U(20, 10000);
				rand1 = U(generator);
				rand2 = U(generator);
			}

			hash_table(const std::initializer_list<key_type>& list)
				: hash_table()

			{
				for (const auto& elem : list) {
					insert(elem);
				}
			}

			hash_table(hash_table& oth) :
				elements(oth.elements),
				prehasher(oth.prehasher),
				rand1(oth.rand1),
				rand2(oth.rand2),
				start_pos(oth.start_pos),
				end_pos(oth.end_pos),
				table(oth.table.size()) {

				for (auto& elem : oth) {

					long long int hash_key = hash(elem, table.size());
					table[hash_key].push_front(elem);
				}

			}
			hash_table(hash_table&& oth) :
				elements(0),
				rand1(0),
				rand2(0),
				start_pos(0),
				end_pos(0),
				load_factor(0) {

				table.swap(oth.table);
				std::swap(load_factor, oth.load_factor);
				std::swap(elements, oth.elements);
				std::swap(prehasher, oth.prehasher);
				std::swap(rand1, oth.rand1);
				std::swap(rand2, oth.rand2);
				std::swap(start_pos, oth.start_pos);
				std::swap(end_pos, oth.end_pos);

			}

			hash_table& operator=(const hash_table& rhs) { // needs optimizing

				clear();
				load_factor = rhs.load_factor;
				prehasher = rhs.prehasher;
				rand1 = rhs.rand1;
				rand2 = rhs.rand2;
				elements = rhs.elements;
				start_pos = rhs.start_pos;
				end_pos = rhs.end_pos;

				table.resize(rhs.table.size());

				for (auto elem : rhs) {

					long long int hash_key = hash(elem, table.size());
					table[hash_key].push_front(elem);
				}

				return *this;
			}
			hash_table& operator=(hash_table&& rhs) {
				clear();
				table.swap(rhs.table);
				std::swap(elements, rhs.elements);
				prehasher = rhs.prehasher;
				rand1 = rhs.rand1;
				rand2 = rhs.rand2;
				std::swap(start_pos, rhs.start_pos);
				std::swap(end_pos, rhs.end_pos);
				return *this;
			}

			const_iterator find(const key_type& key) {
				long long int hash_key = hash(table.size(), key);

				bucket_iterator list_iter = table[hash_key].find(key);

				if (list_iter == table[hash_key].end()) {
					return cend();
				}
				else {
					return const_iterator(&table[hash_key], list_iter, this, hash_key);
				}
			}

			const_iterator cbegin() {
				if (empty()) {
					return cend();
				}
				const_iterator it(&table[start_pos], table[start_pos].begin(), this, start_pos);
				return it;
			}

			const_iterator cend() {
				return const_iterator();
			}

			iterator begin() {
				if (empty()) {
					return end();
				}
				iterator it(&table[start_pos], table[start_pos].begin(), this, start_pos);
				return it;
			}

			iterator end() {
				return iterator();
			}

			void insert(const key_type& key) {
				int hash_key = hash(table.size(), key);

				if (table[hash_key].find(key) == table[hash_key].end()) {
					update_start_and_end(hash_key);

					table[hash_key].push_front(key);
					++elements;
					grow();
				}
			}
			void insert(key_type&& key) {
				int hash_key = hash(table.size(), key);

				if (table[hash_key].find(key) == table[hash_key].end()) {
					update_start_and_end(hash_key);

					table[hash_key].push_front(std::move(key));
					++elements;
					grow();
				}
			}
			void insert(const std::initializer_list<key_type>& list) {
				for (const auto& element : list) {
					insert(element);
				}
			}
			void insert(std::initializer_list<key_type>&& list) {
				for (const auto& element : list) {
					insert(std::move(element));
				}
			}


			void erase(const key_type& key) {
				if (empty()) {
					return;
				}

				int hash_key = hash(table.size(), key);

				if (table[hash_key].erase(key)) {

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

			size_t buckets() {
				return table.size();
			}

			size_t size() {
				return elements;
			}

			bool empty() {
				return elements == 0;
			}
			void rehash(unsigned int n) {
				if (n <= elements) {
					return;
				}
				else if (n < table.size() && load_factor * 0.8 >= (float)elements / n) {

					start_pos = 2147483647;
					end_pos = -1;

					std::vector<Bucket> temp(n);
					for (auto& bucket : table) {
						for (auto& element : bucket) {

							int hash_key = hash(temp.size(), element);

							update_start_and_end(hash_key);

							temp[hash_key].push_front(element);
						}
					}

					table = std::move(temp);
				}
				else {

					start_pos = INT_MAX;
					end_pos = -1;
					int possibility = (int)((float)table.size() * 1.15f);
					if (possibility > n) {
						n = possibility;
					}
					std::vector< Bucket > temp(n);
					for (auto& bucket : table) {
						for (auto& element : bucket) {

							int key = hash(temp.size(), element);

							update_start_and_end(key);

							temp[key].push_front(element);
						}
					}
					table = std::move(temp);
				}
			}

			void clear() {
				std::vector<Bucket> new_table(starting_size);
				table = std::move(new_table);
				elements = 0;
				start_pos = INT_MAX;
				end_pos = -1;
			}

			void reserve(unsigned int n) {
				if (n < (float)starting_size *load_factor) {
					return;
				}
				unsigned int new_size = (unsigned int)((float)(n + 1) / load_factor);
				rehash(new_size);
			}


		private:

			void update_start_and_end(int hash_key) {
				if (start_pos > hash_key) {
					start_pos = hash_key;
				}
				if (end_pos < hash_key) {
					end_pos = hash_key;
				}
			}

			void update_start_pos() {
				if (empty()) {
					start_pos = INT_MAX;
					return;
				}
				if (elements == 1) {
					start_pos = end_pos;
					return;
				}
				while (start_pos < end_pos) {
					if (! table[++start_pos].empty()) {
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
					if (! table[--end_pos].empty()) {
						return;
					}
				}
			}

			int hash(size_t size, const key_type& key) {

				long long int prehash_key = prehasher(key);
				int prime = 8188057;

				long long int new_key = (rand1 * prehash_key + rand2);
				new_key %= prime;
				new_key %= size;
				return (int)new_key;
			}

			void shrink() {
				if (table.size() > starting_size && (float)table.size() * load_factor >= elements * 4) {

					start_pos = INT_MAX;
					end_pos = -1;

					std::vector<Bucket> temp((int)((float)table.size() * load_factor / 2));
					for (auto& bucket : table) {
						for (auto& element : bucket) {

							int hash_key = hash(temp.size(), element);

							update_start_and_end(hash_key);

							temp[hash_key].push_front(element);
						}
					}

					table = std::move(temp);
				}
			}


			void grow() {

				if ((float)elements / table.size() >= load_factor) {

					start_pos = 2147483647;
					end_pos = -1;

					std::vector<Bucket > temp(2 * table.size());
					for (auto& bucket : table) {
						for (auto& element : bucket) {

							int hash_key = hash(temp.size(), element);

							update_start_and_end(hash_key);


							temp[hash_key].push_front(element);
						}
					}
					table = std::move(temp);

				}
			}
	};

	template<typename key_type, typename prehash, typename Compare>
	const unsigned int hash_table<key_type, prehash, Compare>::starting_size = 20;

}

#endif