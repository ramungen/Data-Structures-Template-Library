#ifndef TRIE_H
#define TRIE_H
#include <unordered_map>
#include <unordered_set>
#include <stack>

namespace dsl {
	class Trie {

	// forward declarations
		struct Node;
		class TrieHash;
		struct Symbol;

	// public interface
	public:
		Trie() : root(new Node) {}

		size_t size() {	return size_; }
		void erase(const std::string& word);
		bool contains(const std::string& word) const;
		void insert(const std::string& word);

	// utility functions
	private:
		void DeleteNodes(std::stack<std::pair<Node*, Symbol> >& toDelete);


	// private members
	private:

		size_t size_;
		Node* root;


	// helper classes
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

	};

}

#endif