#pragma once
#include "AVL_tree.h"

namespace data_structures {

	template <typename key_type, typename val_type>
	class map {
	private:
		using pair = std::pair<key_type, val_type>;
	public:
		using iterator = typename AVL_tree<pair>::tree_iterator;

		map() :{}
		map(const map<key_type, val_type>& other)  : tree(other) {}
		~map() {
			clear();
		}

		
		void insert(const pair& value) {
			tree.insert(value);
		}
		void insert(pair&& value) {
			tree.insert(std::forward(value));
		}
		void erase(const key_type key) {
			//tree.erase(key);
		}

		iterator find(const key_type& key) {
			return 
		}

		void clear() {
			tree.clear();
		}

		iterator begin() {
			return tree.begin();
		}
		iterator end() {
			return tree.end();
		}

		size_t size() {
			return tree.size();
		}

		bool empty() {
			return tree.empty();
		}

		val_type& operator[](const key_type& key) {

			if (find(key) == end()) {
				val_type value;
				if (std::is_arithmetic<val_type>::value) {
					value = 0;
				}
				
				insert(std::make_pair(key, value));
			}
			return find(key)->second;

		}

	private:

		AVL_tree<pair> tree;
	};

}