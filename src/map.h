#pragma once
#include "AVL_tree.h"

namespace data_structures {

	template <typename key_type, typename val_type, typename compare = std::less<key_type> >
	class map {
	private:
		using pair = std::pair<const key_type, val_type>;

		// functor that compares pairs by their keys
		class map_compare {
		private:
			compare comp;
		public:
			bool operator()(const pair& lhs, const pair& rhs) const {
				return comp(lhs.first, rhs.first);
			}
		};

	public:
		using iterator = typename implementation::AVL_tree<pair, map_compare>::iterator;
		using const_iterator = typename implementation::AVL_tree<pair, map_compare>::const_iterator;

		~map() {
			clear();
		}
		map() {}

		map(const map& oth) {
			this->tree = oth->tree;
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
		void insert(const std::initializer_list<pair>& list) {
			for (auto elem : list) {
				insert(elem);
			}
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
			return it->second;
		}

		const_iterator find(const key_type& key) const {
			val_type dummy;
			return tree.find(std::make_pair(key, dummy));
		}

		const_iterator cbegin() {
			return tree.cbegin();
		}

		const_iterator cend() {
			return tree.cend();
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
		implementation::AVL_tree<pair, map_compare> tree;
	};

}