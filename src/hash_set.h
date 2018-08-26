#ifndef HASHSET_H
#define HASHSET_H

#include "hash_table.h"

namespace data_structures {

	template <typename key_type,
		typename prehash = std::hash<key_type>, class Compare = std::less<key_type> >

		class hash_set {
		public:
			using iterator = typename hash_table<key_type>::iterator;
			using const_iterator = typename hash_table<key_type>::const_iterator;
			hash_set() {}

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

			size_t buckets() {
				return table.buckets():
			}

			bool empty() {
				return table.empty();
			}

			void erase(const key_type& key) {
				table.erase(key);
			}
			const_iterator find(const key_type& key) {
				return table.find(key);
			}

			void insert(key_type&& key) {
				table.insert(std::forward<key_type>(key));
			}

			void insert(const key_type& key) {
				table.insert(key);
			}

			void insert(const std::initializer_list<key_type>& list) {
				table.insert(list);
			}

			void insert(std::initializer_list<key_type>&& list) {
				for (const auto& element : list) {
					insert(std::forward<key_type>(element));
				}
			}

			void rehash(unsigned int n) {
				table.rehash(n);
			}

			void reserve(unsigned int n) {
				table.reserve(n);
			}

			size_t size() {
				return table.size();
			}

			void clear() {
				table.clear();
			}

		private:
			hash_table<key_type, prehash, Compare> table;
	};


}
#endif
