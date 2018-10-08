
#include "Trie.h"
using dsl::Trie;

void Trie::erase(const std::string& word) {
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

bool Trie::contains(const std::string& word) const {

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

void Trie::insert(const std::string& word) {
	bool alreadyExisted = true;
	++size_;

	Node* current = root;
	bool isEnd = false;

	size_t counter = 0;
	for (auto& letter : word) {

		// isEnd is true when we reach the last letter of the word
		isEnd = (counter++ == word.length() - 1);

		Symbol symbol(letter, isEnd);

		// if the symbol does not exist yet
		if (current->symbols.find(symbol) == current->symbols.end()) {
			alreadyExisted = false;
			current->symbols[symbol] = new Node;
		}
		current = current->next(symbol);
	}

	// we did not actually insert anything so increase the size back to normal
	if (alreadyExisted) {
		--size_;
	}

}

void Trie::DeleteNodes(std::stack<std::pair<Node*, Symbol> >& toDelete) {

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