#ifndef TRIE_H
#define TRIE_H
#include <unordered_map>
#include <unordered_set>
#include <stack>

namespace data_structures {
	class Trie {
	private:


		struct Symbol {
			Symbol(char ch, bool end) : character(ch), endOfWord(end) {}

			bool operator==(const Symbol& rhs) const {
				return this->character == rhs.character && this->endOfWord == rhs.endOfWord;
			}

			char character;
			bool endOfWord;
		};


		class TrieHash {
		private:
			std::hash<char> Hash;
		public:
			size_t operator()(const Symbol& symbol) const {
				return Hash(symbol.character);
			}
		};

		struct Node {

			std::unordered_map<Symbol, Node*, TrieHash> symbols;

			inline Node* next(Symbol symbol) const {
				return symbols.at(symbol);
			}
		};



	public:

		Trie() : root(new Node) {

		}

		size_t size() {
			return size_;
		}

		void erase(const std::string& word) {
			--size_;

			Node* current = root;
			bool isEnd = false;

			std::stack<std::pair<Node*, Symbol> > toDelete;

			size_t counter = 0;
			for (auto& letter : word) {
				isEnd = (counter++ == word.length() - 1);

				Symbol symbol(letter, isEnd);

				// if the symbol is not found, then
				// the word is not present in the trie
				if (current->symbols.find(symbol) == current->symbols.end()) {
					// no deletion happened so increase the size to previous
					++size_;
					return;
				}

				// we fill not delete those Symbols that have neighbours
				if (current->symbols.size() < 2) {
					toDelete.push({ current, symbol });
				}

				current = current->next(symbol);
			}

			DeleteNodes(toDelete);
		}

		bool contains(const std::string& word) const {

			Node* current = root;
			bool isEnd = false;

			size_t counter = 0;
			for (auto& letter : word) {
				isEnd = (counter++ == word.length() - 1);

				Symbol symbol(letter, isEnd);
				if (current->symbols.find(symbol) == current->symbols.end()) {
					return false;
				}

				current = current->next(symbol);
			}

			return true;
		}


		void insert(const std::string& word) {
			bool alreadyExisted = true;
			++size_;

			Node* current = root;
			bool isEnd = false;

			size_t counter = 0;
			for (auto& letter : word) {
				isEnd = (counter++ == word.length() - 1);

				Symbol symbol(letter, isEnd);

				// if the symbol does not yet exist
				if (current->symbols.find(symbol) == current->symbols.end()) {
					alreadyExisted = false;
					current->symbols[symbol] = new Node;
				}
				current = current->symbols.at(symbol);
			}

			// we did not actually insert anything so increase the size back to normal
			if (alreadyExisted) {
				--size_;
			}

		}

		// utility functions
	private:

		void DeleteNodes(std::stack<std::pair<Node*, Symbol> >& toDelete) {

			while (!toDelete.empty()) {

				auto[node, symbol] = toDelete.top();
				toDelete.pop();

				Node* nodeToDelete = node->next(symbol);

				// if it has no neighbours
				if (nodeToDelete->symbols.size() == 0) {
					delete nodeToDelete;
					node->symbols.erase(symbol);
				}
			}
		}

		// private members
	private:

		size_t size_;
		Node* root;
	};

}

#endif