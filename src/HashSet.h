
#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <initializer_list>
#include "SList.h"



template <typename val_type>
class HashSet {
public:
	HashSet(const std::initializer_list<val_type>& list);
	HashSet();
	~HashSet();
	int size();
	bool empty();
	void insert(const val_type& value);
	void erase(const val_type& value);
	bool contains(const val_type& value);
	void traverse();
	unsigned int buckets();
private:
	
	long long int preHash(const val_type& value);
	long long int hash(long long int prehashKey, size_t size);

	void grow();
	void shrink();

private:

	std::vector< SList<val_type> > set;
	unsigned int elements;
	static const unsigned int startingSize;
	int a, b;
};



template<typename val_type>
const unsigned int HashSet<val_type>::startingSize = 20;

template<typename val_type>
HashSet<val_type>::HashSet() : elements(0), set(startingSize) {

	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> U(20, 10000);
	a = U(generator);
	b = U(generator);
}
template<typename val_type>
HashSet<val_type>::HashSet(const std::initializer_list<val_type>& list)
	: HashSet<val_type>()

{
	for (const auto& name : list) {
		insert(name);
	}
}
template<typename val_type>
HashSet<val_type>::~HashSet() {
}

template<typename val_type>
bool HashSet<val_type>::contains(const val_type& value) {
	long long int prehashKey = preHash(value);
	long long int trueKey = hash(prehashKey, set.size());
	return set[trueKey].find(value);
}

template<typename val_type>
long long int HashSet<val_type>::preHash(const val_type& value) {
	int key = 7;
	for (const auto& letter : value) {
		key = key * 31 + (int)letter;
	}
	return std::abs(key);
}

template<typename val_type>
long long int HashSet<val_type>::hash(long long int prehashKey, size_t size) {
	long long int oldKey = prehashKey;
	int prime = 8188057;

	long long int trueKey = (a * oldKey + b);
	trueKey %= prime;
	trueKey %= size;
	return trueKey;
}

template<typename val_type>
void HashSet<val_type>::insert(const val_type& value) {
	long long int prehashKey = preHash(value);
	long long int trueKey = hash(prehashKey, set.size());

	if (!set[trueKey].find(value)) {
		set[trueKey].push_front(value);
		++elements;
		grow();
	}
}

template<typename val_type>
void HashSet<val_type>::erase(const val_type& value) {
	if (empty()) {
		return;
	}

	long long int prehashKey = preHash(value);
	long long int trueKey = hash(prehashKey, set.size());
	if (set[trueKey].erase(value)) {
		--elements;
		shrink();
	}
}

template<typename val_type>
void HashSet<val_type>::shrink() {
	if (set.size() > startingSize && set.size() >= elements * 4) {
		std::vector<SList<val_type>> temp(set.size() / 2);
		for (const auto& bucket : set) {
			for (const auto& element : bucket) {

				long long int prehashKey = preHash(element);
				long long int trueKey = hash(prehashKey, (unsigned int)temp.size());
				temp[trueKey].push_front(element);
			}
		}
		set = std::move(temp);
	}
}


template<typename val_type>
void HashSet<val_type>::grow() {
	if (set.size() > elements) {
		return;
	}
	std::vector<SList< val_type > > temp(2 * set.size());
	for (const auto& bucket : set) {
		for (const auto& element : bucket) {

			long long int prehashKey = preHash(element);
			long long int trueKey = hash(prehashKey, (unsigned int)temp.size());
			temp[trueKey].push_front(element);
		}
	}
	set = std::move(temp);

}

template<typename val_type>
unsigned int HashSet<val_type>::buckets() {
	return (unsigned int)set.size();
}

template<typename val_type>
void HashSet<val_type>::traverse() {
	for (const auto& bucket : set) {
		bucket.print();
	}
}

template<typename val_type>
int HashSet<val_type>::size() {
	return elements;
}

template<typename val_type>
bool HashSet<val_type>::empty() {
	return elements == 0;
}




#endif
