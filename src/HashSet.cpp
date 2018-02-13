
#include "HashSet.h"
#include "SList.h"

const unsigned int HashSet::startingSize = 20;

HashSet::HashSet() : elements(0), set(startingSize) {

	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> U(20, 10000);
	a = U(generator);
	b = U(generator);
}

HashSet::HashSet(const std::initializer_list<std::string>& list) 
	: HashSet()

{
	for (const auto& name : list) {
		insert(name);
	}
}

HashSet::~HashSet() {
}

bool HashSet::contains(const std::string& value) {
	long long int prehashKey = preHash(value);
	long long int trueKey = hash(prehashKey, set.size());
	return set[trueKey].find(value);
}

long long int HashSet::preHash(const std::string& value) {
	int key = 7;
	for (const auto& letter : value) {
		key = key * 31 + (int)letter;
	}
	return std::abs(key);
}

long long int HashSet::hash(long long int prehashKey, size_t size) {
	long long int oldKey = prehashKey;
	int prime = 8188057;

	long long int trueKey = (a * oldKey + b);
	trueKey %= prime;
	trueKey %= size;
	return trueKey;
}


void HashSet::insert(const std::string& value) {
	long long int prehashKey = preHash(value);
	long long int trueKey = hash(prehashKey, set.size());

	if (!set[trueKey].find(value)) {
		set[trueKey].push_front(value);
		++elements;
		grow();
	}
}

void HashSet::erase(const std::string& value) {
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


void HashSet::shrink() {
	if (set.size() > startingSize && set.size() >= elements * 4) {
		std::vector<SList<std::string>> temp(set.size() / 2);
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

void HashSet::grow() {
	if (set.size() > elements) {
		return;
	}
	std::vector<SList<std::string>> temp(2 * set.size());
	for (const auto& bucket : set) {
		for (const auto& element : bucket) {

			long long int prehashKey = preHash(element);
			long long int trueKey = hash(prehashKey, (unsigned int)temp.size());
			temp[trueKey].push_front(element);
		}
	}
	set = std::move(temp);

}

unsigned int HashSet::buckets() {
	return (unsigned int)set.size();
}

void HashSet::traverse() {
	for (const auto& bucket : set) {
		bucket.print();
	}
}

int HashSet::size() {
	return elements;
}

bool HashSet::empty() {
	return elements == 0;
}