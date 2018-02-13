
#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <initializer_list>
#include "SList.h"


class HashSet {
public:
	HashSet(const std::initializer_list<std::string>& list);
	HashSet();
	~HashSet();
	int size();
	bool empty();
	void insert(const std::string& value);
	void erase(const std::string& value);
	bool contains(const std::string& value);
	void traverse();
	unsigned int buckets();
private:
	
	long long int preHash(const std::string& value);
	long long int hash(long long int prehashKey, size_t size);

	void grow();
	void shrink();

private:

	std::vector< SList<std::string> > set;
	unsigned int elements;
	static const unsigned int startingSize;
	int a, b;
};


#endif
