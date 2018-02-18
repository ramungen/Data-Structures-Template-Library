
#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <initializer_list>
#include "SList.h"

namespace {
	template <typename str> // compiler calls this one if both are strings
	bool isString(const str& s1, const str& s2) {
		return true;
	}

	template<typename str, typename oth_type> // calls this one if not
	bool isString(const str& s1, const oth_type& other) {
		return false;
	}

	template <typename not_string>
	std::string stringify(not_string val) {
		std::string str;
		
	}
}




template <typename val_type>
struct PreHash {
	size_t operator()(const val_type& value) const {
		size_t prehashCode = std::hash<val_type>{}(value);
		return prehashCode;
	}
};

template <typename val_type, typename prehash = PreHash<val_type> >
class HashSet {
private:

	std::vector< SList<val_type> > set;
	unsigned int elements;
	static const unsigned int startingSize;
	prehash prehasher;
	int a, b;

public:

	HashSet() : 
		elements(0), 
		set(startingSize) {

		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<int> U(20, 10000);
		a = U(generator);
		b = U(generator);
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

	bool contains(const val_type& value) {
		long long int prehashKey = prehasher(value);
		long long int trueKey = hash(prehashKey, set.size());
		return set[trueKey].find(value);
	}


	void insert(const val_type& value) {
		long long int prehashKey = prehasher(value);
		long long int trueKey = hash(prehashKey, set.size());

		if (!set[trueKey].find(value)) {
			set[trueKey].push_front(value);
			++elements;
			grow();
		}
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

		long long int trueKey = (a * oldKey + b);
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
