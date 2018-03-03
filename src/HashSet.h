
#ifndef HASHSET_H
#define HASHSET_H

#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <initializer_list>
#include "SList.h"


/*
KNOWN BUGS:

*/
template <typename val_type, 
	typename prehash = std::hash<val_type> >

	class HashSet {

	/*
		NEED TO:
		1. *DONE* keep track of the position of the first non-empty bucket
		2. *DONE* keep track of the position of the last non-empty bucket
		3. *DONE* update begin() and end()
		*/
	private:
		typedef typename SList<val_type>::iterator list_iterator;


		class Bucket {
			/*
			NEED TO :
			*/
		public:

			val_type& data() {
				return *bucket_iterator;
			}
			bool empty() {
				if (bucket_list.begin() == bucket_list.end()) {
					return true;
				}
				return false;
			}

			void push_front(const val_type& value) {
				bucket_list.push_front(value);
			}

			bool erase(const val_type& value) {
				return bucket_list.erase(value);
			}


			list_iterator find(const val_type& value) {
				return bucket_list.find(value);
			}

			list_iterator last() {
				return bucket_list.end();
			}
			
			Bucket()
			{}

			Bucket(Bucket& other, list_iterator iter) : // needs replacing, inefficient
			bucket_list(other.bucket_list),
			bucket_iterator(iter)
			{}

			~Bucket() {}

		public:
			SList<val_type> bucket_list;
			list_iterator bucket_iterator;

		};

	private:
		
		std::vector< Bucket > set;
		unsigned int elements;
		static const unsigned int startingSize;
		prehash prehasher;
		int rand1, rand2;

		int start_pos;
		int end_pos;
	

	private:

		template<typename val_type>
		class forward_iterator :
			public std::iterator<std::forward_iterator_tag, Bucket> {
			/*
				THINGS TO DO:
				1. make bucket_iterator from Bucket class a part of forward_iterator class
			*/
		public:

			forward_iterator() : ptr(nullptr) {}
			forward_iterator(pointer p) : ptr(p) {}
			forward_iterator(reference p) : ptr(&p) {}
			forward_iterator(const forward_iterator* rhs) : ptr(rhs->ptr) {}
			forward_iterator(const forward_iterator& rhs) : ptr(rhs.ptr) {}
			/*forward_iterator& operator=(const forward_iterator& rhs) {
				ptr = rhs.ptr;
				std::cout << "asfdf" << '\n';
				return *this;

			}*/
			forward_iterator& operator=(forward_iterator rhs) {
				ptr = rhs.ptr;
				return *this;
			}
			forward_iterator operator++() {
				if (ptr == nullptr) {
					throw std::exception("error advancing end iterator");
				}
				if (ptr->bucket_iterator != ptr->bucket_list.end()) {
					++(ptr->bucket_iterator);
				}
				if (ptr->bucket_iterator == ptr->bucket_list.end()) {
					while (parent->set[pos].empty()) {
						++pos;
						if (pos == parent->size.size()) {
							ptr = nullptr;
							return *this;
						}
					}
					ptr = &(parent->set[pos]);
				}

				return *this;
			}
			forward_iterator operator++(int dum) {
				if (ptr == nullptr) {
					throw std::exception("error advancing end iterator");
				}
				forward_iterator temp(this);
				++(this);
				return temp;
			}
			bool operator ==(const forward_iterator& rhs) const {
				return ptr == rhs.ptr;
			}
			bool operator !=(const forward_iterator& rhs) const {
				return ptr != rhs.ptr;
			}
			val_type& operator*() {
				if (ptr == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return ptr->data();
			}
			val_type& operator->() {
				if (ptr == nullptr) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return ptr->data();
			}

		private:
			pointer ptr;
			list_iterator ptr_it;
			HashSet* parent;
			unsigned int pos;
		};


	public:

		typedef forward_iterator<val_type> iterator;
		typedef forward_iterator<const val_type> const_iterator;

		HashSet() :
			elements(0),
			start_pos(2147483647),
			end_pos(-1),
			set(startingSize) {

			std::random_device rd;
			std::mt19937 generator(rd());
			std::uniform_int_distribution<int> U(20, 10000);
			rand1 = U(generator);
			rand2 = U(generator);
		}

		HashSet(const std::initializer_list<val_type>& list)
			: HashSet()

		{
			for (const auto& name : list) {
				insert(name);
			}
		}

		~HashSet() {

		}


		iterator find(const val_type& value) {
			long long int prehashKey = prehasher(value);
			long long int trueKey = hash(prehashKey, set.size());

			list_iter = set[trueKey].find(value);
			
			if (list_iter == set[trueKey].last()) {
				return end();
			}
			else {
				Bucket* temp_bucket(set[trueKey], list_iter); // might not work
				iterator tmp(temp_bucket);
				return tmp;
			}
		}

		iterator end() { // try return iterator(); when have time
			iterator last;
			return last;
		}

		void insert(const val_type& value) {
			long long int prehashKey = prehasher(value);
			long long int trueKey = hash(prehashKey, set.size());

			if (set[trueKey].find(value) == set[0].last()) {

				update_start_and_end(trueKey);

				set[trueKey].push_front(value);
				++elements;
				grow();
			}
		}

		iterator begin() { 
			iterator it(set[start_pos]);
			return it;
		}

		public:

		void erase(const val_type& value) { 
			if (empty()) {
				return;
			}

			long long int prehashKey = prehasher(value);
			long long int trueKey = hash(prehashKey, set.size());
			if (set[trueKey].erase(value)) {
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
				start_pos = last_pos;
				return;
			}
			while (start_pos < last_pos) {
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


		long long int hash(long long int prehashKey, size_t size) {
			long long int oldKey = prehashKey;
			int prime = 8188057;

			long long int trueKey = (rand1 * oldKey + rand2);
			trueKey %= prime;
			trueKey %= size;
			return trueKey;
		}

		void shrink()  { 
			if (set.size() > startingSize && set.size() >= elements * 4) {
				start_pos = 2147483647;
				end_pos = -1;
				std::vector<SList<val_type>> temp(set.size() / 2);
				for (const auto& bucket : set) {
					for (const auto& element : bucket) {

						long long int prehashKey = prehasher(element);
						long long int trueKey = hash(prehashKey, (unsigned int)temp.size());

						update_start_and_end(trueKey);

						temp[trueKey].push_front(element);
					}
				}
				set = std::move(temp);
			}
		}


		void grow() { 
			if (set.size() > elements) {
				return;
			}

			start_pos = 2147483647;
			end_pos = -1;

			std::vector<SList< val_type > > temp(2 * set.size());
			for (const auto& bucket : set) {
				for (const auto& element : bucket) {

					long long int prehashKey = prehasher(element);
					long long int trueKey = hash(prehashKey, (unsigned int)temp.size());

					update_start_and_end(trueKey);

					temp[trueKey].push_front(element);
				}
			}
			set = std::move(temp);

		}


};

template<typename val_type, typename prehash = PreHash<val_type> >
const unsigned int HashSet<val_type, prehash>::startingSize = 20;

#endif
