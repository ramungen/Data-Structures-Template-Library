#pragma once
#include "AVL_tree.h"

namespace data_structures {

	template <typename val_type, class compare = std::less<val_type> >
	class set {

	public:
		using iterator = typename AVL_tree<val_type>::tree_iterator;

		iterator begin() {
			return tree.begin();
		}
		iterator end() {
			return tree.end();
		}
		iterator find(const val_type& value) {
			return tree.find(value);
		}

		size_t size() {
			return tree.size();
		}
		void insert(const val_type& value) {
			tree.insert(value);
		}
		void insert(val_type&& value) {
			tree.insert(std::move(value));
		}
		void erase(const val_type& key) {
			tree.erase(key);
		}
		size_t count(const val_type& key) {
			return tree.find(key) == end() ? 0 : 1;
		}

		void clear() {
			tree.clear();
		}

		~set() {
			clear();
		}
		

	private:
		AVL_tree<val_type, compare> tree;
	};

}
