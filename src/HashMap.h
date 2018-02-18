#ifndef HASHMAP_H
#define HASHMAP_H


#include <functional>
#include <random>
#include <utility>
#include <vector>
#include <iostream>
#include <initializer_list>
#include "SList.h"

template<typename key_type, typename val_type, 
	typename prehash = std::hash<key_type> >

	class HashMap {

		private:
			struct pair {
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
				key_type first;
				val_type second;
				bool operator==(const pair& rhs) {
					return this->first == rhs.first;
				}
				//friend std::ostream& operator<<(std::ostream& os, const pair& p);
			};

			//std::ostream& operator<<(std::ostream& os, const pair& p) {
				//return o << p.first << '(' << p.second << ')';
			//}

			std::vector< SList<pair> > map;
			unsigned int elements;
			static const unsigned int startingSize;
			prehash prehasher;
			int rand1, rand2;

		public:

			HashMap() :
				elements(0),
				map(startingSize) {

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

			bool contains(const key_type& value) {
				long long int prehashKey = prehasher(value);
				long long int trueKey = hash(prehashKey, set.size());
				pair p(value);
				return set[trueKey].find(p);
			}

			void insert(const std::pair<key_type, val_type>& elem) {
				long long int prehashKey = prehasher(elem.first);
				long long int trueKey = hash(prehashKey, map.size());
				pair p(elem);
				if (!map[trueKey].find(p)) {
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
				long long int trueKey = hash(prehashKey, set.size());
				pair p(value);
				if (set[trueKey].erase(p)) {
					--elements;
					shrink();
				}
			}

			unsigned int buckets() {
				return (unsigned int)set.size();
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

			long long int hash(long long int prehashKey, size_t size) {
				long long int oldKey = prehashKey;
				int prime = 8188057;

				long long int trueKey = (rand1 * oldKey + rand2);
				trueKey %= prime;
				trueKey %= size;
				return trueKey;
			}

			void shrink() {
				if (map.size() > startingSize && map.size() >= elements * 4) {
					std::vector<SList<pair>> temp(set.size() / 2);
					for (const auto& bucket : map) {
						for (const auto& element : bucket) {

							long long int prehashKey = prehasher(element);
							long long int trueKey = hash(prehashKey, (unsigned int)temp.size());
							temp[trueKey].push_front(element);
						}
					}
					map = std::move(temp);
				}
			}


			void grow() {
				if (map.size() > elements) {
					return;
				}
				std::vector<SList< pair > > temp(2 * map.size());
				for (const auto& bucket : map) {
					for (const auto& element : bucket) {

						long long int prehashKey = prehasher(element.first);
						long long int trueKey = hash(prehashKey, (unsigned int)temp.size());
						temp[trueKey].push_front(element);
					}
				}
				map = std::move(temp);

			}


	};

	template<typename key_type, typename val_type,
		typename prehash = std::hash<key_type> >
	const unsigned int HashMap<key_type, val_type, prehash>::startingSize = 20;

#endif
