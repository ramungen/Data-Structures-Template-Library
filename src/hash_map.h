#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "hash_table.h"


namespace data_structures {

	template<typename key_type, typename val_type,
		typename prehash = std::hash<key_type> , class Compare = std::less<key_type> >

		class hash_map {

		private:
			using pair = std::pair<const key_type, val_type>;

			// workaround to hash elements by their keys
			class map_prehash {
				prehash prehasher;

			public:
				size_t operator()(const pair& elem) {
					return prehasher(elem.first);
				}
			};
			// workaround to compare elements by their keys
			class map_compare {
			private:
				Compare comp;
			public:
				bool operator()(const pair& lhs, const pair& rhs) const {
					return comp(lhs.first, rhs.first);
				}
			};



		public:
			using iterator = typename implementation::hash_table<pair, map_prehash, map_compare>::iterator;
			using const_iterator = typename implementation::hash_table<pair, map_prehash, map_compare>::const_iterator;

			hash_map(float factor = 0.75f) : table(factor) {}

			hash_map(const std::initializer_list<pair>& list) :
			table(list)
			{}

			hash_map(hash_map& oth) :
			{
			}
			hash_map(hash_map&& oth) : table(std::move(oth.table)) {}

			hash_map& operator=(hash_map& rhs) { // needs optimizing
			}
			hash_map& operator=(hash_map&& rhs) {
				table = std::move(oth.table);
			}

			~hash_map() {

			}
		iterator begin() {
				return table.begin();
			}
			iterator end() {
				return iterator();
			}

			const_iterator cbegin() {
				return table.cbegin();
			}
			const_iterator cend() {
				return const_iterator();
			}

			const_iterator find(const key_type& key) {
				val_type dummy;
				return table.find(std::make_pair(key, dummy));
			}

			val_type& at(const key_type& key) {
				auto it = table.find(key);
				if (it == end()) {
					throw std::out_of_range("the element is not present in the container");
				}
				return it->second;
			}
			void insert(const pair& elem) {
				table.insert(elem);
			}

			void insert(pair&& elem) {
				table.insert(std::forward<pair>(elem));
			}

			void insert(const std::initializer_list<key_type>& list) {
				for (auto elem : list) {
					table.insert(elem);
				}
			}
			void insert(std::initializer_list<key_type>&& list) {
				for (auto elem : list) {
					table.insert(std::move(elem));
				}
			}

			size_t count(const key_type& key) {
				return table.find(key) == end() ? 0 : 1;
			}

			val_type& operator[](const key_type& key) {
				/*long long int hash_key = hash(table.size(), key);

				return table[hash_key].create_if_not_found(key);*/
				// TODO: currently this is inefficient, so make it more efficient
				//return table.try_return_value(key);
				auto it = find(key);
				if (it == cend()) {
					val_type value;
					if (std::is_arithmetic<val_type>::value) {
						value = 0;
					}
					insert(std::make_pair(key, value));
					it = find(key);
				}
				return(val_type)(find(key)->second);
			}


			void erase(const key_type& key) {
				val_type dummy;
				table.erase(std::make_pair(key, dummy));
			}


			void rehash(unsigned int n) {
				table.rehash(n);
			}
			void clear() {
				table.clear();
			}

			void reserve(unsigned int n) {
				table.reserve();
			}


			size_t buckets() {
				table.buckets();
			}

			int size() {
				return table.size();
			}

			bool empty() {
				return table.size();
			}
		private:

			implementation::hash_table<pair, map_prehash, map_compare> table;
	};
}
#endif


