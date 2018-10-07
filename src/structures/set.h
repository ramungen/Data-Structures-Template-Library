#ifndef SET_H
#define SET_H

#include "AVL_tree.h"

namespace dsl {

	template <typename val_type, class compare = std::less<val_type> >
	class set {

	public:
		using iterator = typename implementation::AVL_tree<val_type, compare>::iterator;
		using const_iterator = typename implementation::AVL_tree<val_type, compare>::const_iterator;

		set() {}

		set(const set& oth) {
			this->tree = oth->tree;
		}

		set(const std::initializer_list<val_type>& list) : tree(list) {}

		set(set&& oth) {
			tree(std::move(oth.tree));
		}
		set& operator=(set&& oth) {
			tree = std::move(oth.tree);
		}

		const_iterator cbegin() {
			return tree.cbegin();
		}

		const_iterator cend() {
			return tree.cend();
		}

		iterator begin() {
			return tree.begin();
		}
		iterator end() {
			return tree.end();
		}
		const_iterator find(const val_type& value) {
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
			return find(key) == cend() ? 0 : 1;
		}

		void clear() {
			tree.clear();
		}

		~set() {
			clear();
		}


	private:
		implementation::AVL_tree<val_type, compare> tree;
	};

}


#endif