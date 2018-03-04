#ifndef HASHMAP_H
#define HASHMAP_H


#include <functional>
#include <random>
#include <utility>
#include <vector>
#include <iostream>
#include <initializer_list>
#include "SList.h"

/*
Things that still need to be implemented:
1. Rvalue reference support
2. Iterator for map class
3. operator[]

*/


/*
 THINGS TO DO:
 1. *done* track start and end positions
 2. implement iterator support
 3. replace contains() with find()
 3. add the same functionality as in set()
 4. make pair have std::pair member variable instead of first/second, so more efficient
 5. make key non-modifiable and value modifiable when accessing them via iterators
*/

template<typename key_type, typename val_type, 
	typename prehash = std::hash<key_type> >

	class HashMap {

		private:
			
			

			struct pair { // std::pair class does not do the job because I would have to 
						  //rewrite SList class to accept comparator
				pair() {

				}
				~pair() {};
				pair(const key_type& val) {
					first = val;
				}
				pair(const std::pair<key_type, val_type>& elem) {
					first = elem.first;
					second = elem.second;
				}
				std::pair<key_type, val_type>& to_std_pair() {
					return std::make_pair(first, second);
				}
				key_type first;
				val_type second;
				bool operator==(const pair& rhs) {
					return first == rhs.first;
				}
				bool operator!=(const pair& rhs) {
					return first != rhs.first;
				}
			};
			typedef typename SList<pair>::iterator list_iterator;


			std::vector< SList< pair > > map;
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
			public std::iterator<std::forward_iterator_tag, SList<pair> > {

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
			forward_iterator(pointer p, list_iterator liter, HashMap* prnt, long long int pos_) : ptr(p),
				ptr_it(liter),
				parent(prnt),
				pos(pos_)
			{}
			forward_iterator(reference p, list_iterator liter, HashMap* prnt, long long int pos_) :
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
				if (ptr == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				std::pair<key_type, val_type> p;
				p.first = ptr_it->first;
				p.second = ptr_it->second;
				
				return p;
			}
			std::pair<key_type, val_type>* operator->() const { // change
				if (ptr == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				std::pair<key_type, val_type> p;
				p.first = ptr_it->first;
				p.second = ptr_it->second;
				return &p;
			}

		private:

			pointer ptr;
			list_iterator ptr_it;
			HashMap* parent;
			long long int pos;
		};


	public:

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
				pair p(value);
				return map[trueKey].find(p);
			}

			void insert(const std::pair<key_type, val_type>& elem) {
				long long int prehashKey = prehasher(elem.first);
				long long int trueKey = hash(prehashKey, map.size());
				pair p(elem);
				if (map[trueKey].find(p) == map[trueKey].end()) {

					update_start_and_end(trueKey);

					map[trueKey].push_front(p);
					++elements;
					grow();
				}
			}

			void erase(const val_type& value) {
				if (empty()) {
					return;
				}

				long long int prehashKey = prehasher(value);
				long long int trueKey = hash(prehashKey, map.size());
				pair p(value);
				
				if (map[trueKey].erase(p)) {
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

					start_pos = 2147483647;
					end_pos = -1;

					if (map.size() > starting_size && map.size() >= elements * 4) {
						std::vector<SList<pair>> temp(map.size() / 2);
						for (const auto& bucket : map) {
							for (const auto& element : bucket) {

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

					std::vector<SList< pair > > temp(2 * map.size());
					for (const auto& bucket : map) {
						for (const auto& element : bucket) {

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
