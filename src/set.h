#pragma once
#include "AVL_tree.h"

namespace data_structures {

	template <typename key_type>
	class set {

	public:
		using iterator = typename AVL_tree<key_type>::tree_iterator;

		iterator begin() {
			return tree.begin();
		}
		iterator end() {
			return tree.end();
		}
		iterator find(const key_type& value) {
			return tree.find(value);
		}

		size_t size() {
			return tree.size();
		}
		void insert(const key_type& value) {
			tree.insert(value);
		}
		void insert(key_type&& value) {
			tree.insert(std::move(value));
		}
		void erase(const key_type& key) {
			tree.erase(key);
		}
		size_t count(const key_type& key) {
			return tree.find(key) == end() ? 0 : 1;
		}

	private:
		AVL_tree<key_type> tree;
	};

}
