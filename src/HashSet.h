
#ifndef HASHSET_H
#define HASHSET_H

#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <initializer_list>
#include "SList.h"

/*
	MAKE HASHNODE ACCEPT HASHSET POINTER IN CONSTRUCTOR
	BECAUSE IT DOES NOT HAVE ANY RELATIONSHIP WITH THE OUTER CLASS
*/


/*
	NEED TO REDO EVERYTHING IN TERMS OF HASHNODES
	BECAUSE HANDLING ITERATORS THIS WAY IS TOO DIFFICULT AND INEFFICIENT

*/
template <typename val_type, 
	typename prehash = std::hash<val_type> >

	class HashSet {
	private:
		typedef typename SList<val_type>::iterator list_iterator;

		std::vector< SList<val_type> > set;
		unsigned int elements;
		static const unsigned int startingSize;
		prehash prehasher;
		int rand1, rand2;

		list_iterator list_iter;

	private:

		

		class HashNode {
		public:
			HashNode() :position(0), 
			parent(nullptr)
			{}

			HashNode(HashNode* oth) {
				position = oth->position;
				iter = oth->iter;
				parent = oth->parent;
			}
			HashNode(HashSet* outer) :
				position(0),
				parent(outer)
			{
				iter = parent->set[0].end();
			}

			HashNode(HashSet* outer, int pos) :
				position(pos),
				parent(outer)
			{
				iter = parent->set[0].end();
			}

			HashNode(HashSet* outer, int pos, list_iterator& it) :
				position(pos),
				iter(it),
				parent(outer)
			{}

			HashNode& operator=(const HashNode& rhs) {
				position = rhs.position;
				iter = rhs.iter;
				parent = rhs.parent;
				return *this;
			}

			~HashNode() {}

			val_type& data() {
				return *iter;
			}

			void advance() {
				if (position == parent->set.size()) {
					return;
					}
					if (iter != parent->set[0].end()) { // if the node is not finnished iterating over
						++iter;
					}

					if (iter != parent->set[0].end()) { // we are done
						return;
					}
					else { // jump to next node;
						++position;
						
						iter = parent->set[0].end();
						while (position < parent->set.size()) {
							if (parent->set[position].begin() != parent->set[0].end()) { // jump over empty spots

								break;
							}
							++position;
						}
						if (position == parent->set.size()) {

							iter = parent->set[0].end();

						}
						else {
							iter = (parent->set[position]).begin();
						}
					}
			}

			bool empty() {
				return iter == parent->set[0].end();
			}
		public:
			unsigned int position;
			list_iterator iter;
			HashSet* parent;
		};

		template<typename val_type>
		class forward_iterator :
			public std::iterator<std::forward_iterator_tag, HashNode> {

		public:


			forward_iterator() : ptr(nullptr) {}
			forward_iterator(pointer p) : ptr(p) {}
			forward_iterator(reference p) : ptr(&p) {}
			forward_iterator(const forward_iterator* source) : ptr(source->ptr) {}
			forward_iterator(const forward_iterator& rhs) : ptr(nullptr) {
				ptr = rhs.ptr;
			}
			/*forward_iterator& operator=(const forward_iterator& rhs) {
				ptr = rhs.ptr;
				std::cout << "asfdf" << '\n';
				return *this;

			}*/
			forward_iterator& operator=(forward_iterator rhs) {
				ptr = rhs.ptr;
				return *this;
			}
			forward_iterator& operator++() {
				if (ptr == empty()) {
					throw std::exception("error advancing end iterator");
				}
				ptr->advance();
				if (ptr->empty()) {
					throw std::exception("error advancing end iterator");
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
				if (ptr->empty()) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return ptr->data();
			}
			val_type& operator->() {
				if (ptr->empty()) {
					throw std::exception("error dereferencing an invalid iterator");
				}
				return ptr->data();
			}

		private:
			pointer ptr;
		};

		private:
			HashNode hash_node;


	public:

		typedef forward_iterator<val_type> iterator;
		typedef forward_iterator<const val_type> const_iterator;

		HashSet() :
			elements(0),
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

			if (list_iter == set[trueKey].end()) {
				std::cout << "been here done that " << '\n';
				return end();
			}
			else {
				HashNode temp(this, trueKey, list_iter);
				hash_node = temp;
				iterator tmp(hash_node);
				return tmp;
			}
		}

		iterator end() {
			HashNode temp(this, set.size());
			
			return iterator(temp);
		}

		void insert(const val_type& value) {
			long long int prehashKey = prehasher(value);
			long long int trueKey = hash(prehashKey, set.size());

			if (set[trueKey].find(value) == set[0].end()) {
				set[trueKey].push_front(value);
				++elements;
				grow();
			}
		}

		iterator begin() {
			if (empty()) {
				return end();
			}
			unsigned int position = 0;
			for (; position < set.size(); ++ position) {

				if (!(set[position].empty())) {
					break;
				}
			}
			list_iter = set[position].begin();
			HashNode newnode(this, position, list_iter);
			hash_node = newnode;
			iterator fin(hash_node);
			return fin;
		}
		void erase(const val_type& value) {
			if (empty()) {
				return;
			}

			long long int prehashKey = prehasher(value);
			long long int trueKey = hash(prehashKey, set.size());
			if (set[trueKey].erase(value)) {
				--elements;
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

		long long int hash(long long int prehashKey, size_t size) {
			long long int oldKey = prehashKey;
			int prime = 8188057;

			long long int trueKey = (rand1 * oldKey + rand2);
			trueKey %= prime;
			trueKey %= size;
			return trueKey;
		}

		void shrink() {
			if (set.size() > startingSize && set.size() >= elements * 4) {
				std::vector<SList<val_type>> temp(set.size() / 2);
				for (const auto& bucket : set) {
					for (const auto& element : bucket) {

						long long int prehashKey = prehasher(element);
						long long int trueKey = hash(prehashKey, (unsigned int)temp.size());
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
			std::vector<SList< val_type > > temp(2 * set.size());
			for (const auto& bucket : set) {
				for (const auto& element : bucket) {

					long long int prehashKey = prehasher(element);
					long long int trueKey = hash(prehashKey, (unsigned int)temp.size());
					temp[trueKey].push_front(element);
				}
			}
			set = std::move(temp);

		}


};

template<typename val_type, typename prehash = PreHash<val_type> >
const unsigned int HashSet<val_type, prehash>::startingSize = 20;

#endif
