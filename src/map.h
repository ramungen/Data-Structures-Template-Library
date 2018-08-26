#pragma once
#include "AVL_tree.h"

namespace data_structures {

	template <typename key_type, typename val_type, typename compare = std::less<key_type> >
	class map {
	private:
		using pair = std::pair<key_type, val_type>;
		// compares pairs by their keys
		class map_compare {
		private:
			compare comp;
		public:
			bool operator()(const pair& lhs, const pair& rhs) const {
				return comp(lhs.first, rhs.first);
			}
		};

	public:
		using iterator = typename AVL_tree<pair, map_compare>::tree_iterator;


		map(const map<key_type, val_type>& other)  : tree(other) {}
		~map() {
			clear();
		}
		map() {}

		map(const map& oth) {

		}
		
		map(map&& oth) {
			tree(std::move(oth.tree));
		}
		map& operator=(map&& oth) {
			tree = std::move(oth.tree);
		}
		void insert(const pair& value) {
			tree.insert(value);
		}
		void insert(pair&& value) {
			tree.insert(std::move(value));
		}
		void erase(const key_type& key) {
			val_type dummy;
			tree.erase(std::make_pair(key, dummy));
		}
		val_type& at(const key_type& key) const {
			auto it = find(key);
			if (it == end()) {
				throw std::out_of_range("the specified element does not exist in the container");
			}
			return find(key)->second;
		}

		iterator find(const key_type& key) const {
			val_type dummy;
			return tree.find(std::make_pair(key, dummy));
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

		size_t size() const {
			return tree.size();
		}

		bool empty() const {
			return tree.empty();
		}

		void emplace(pair& Args... args) {

		}

		val_type& operator[](const key_type& key) {
			// TODO: currently this is inefficient, so make it more efficient
			auto it = find(key);
			if (it == end()) {
				val_type value;
				if (std::is_arithmetic<val_type>::value) {
					value = 0;
				}
				insert(std::make_pair(key, value));
				it = find(key);
			}
			return find(key)->second;
			
		}

	private:
		AVL_tree<pair, map_compare> tree;
	};

}