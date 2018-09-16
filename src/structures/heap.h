#ifndef HEAP_H
#define HEAP_H

#include <utility>
#include <functional>
namespace data_structures {
	/*
	by default this is min heap
	make second template parameter std::greater<val_type> for a max heap
	*/

	template <typename val_type, class comp = std::less<val_type>>
	class heap {
		int* array;
		comp compare;
		size_t size_;
		size_t capacity;

	public:

		heap() : size_(0), capacity(3) {
			array = new val_type[3];
		}

		heap(int capacity) : size_(0), capacity(3) {
			array = new val_type[capacity];
		}

		~heap() {
			delete[] array;
		}

		size_t size() { return size_; }

		// extracts the value with the highest priority
		val_type extract_min() {
			if (size_ == 0) {
				throw std::out_of_range("enable to extract min; the heap is empty");
				return val_type();
			}
			val_type min = array[0];
			using std::swap;
			swap(array[0], array[size_ - 1]);
			--size_;
			// if the heap if only one quarter full
			// resize it to 3/2 of the current size
			if (size_ * 4 <= capacity) {
				resize((size_ * 3) / 2);
			}
			fixHeapFromAbove();
			return min;
		}

		// inserts a value into a heap by inserting it to
		// the end of the array and then finding it's proper place
		void insert(const val_type& value) {
			if (size_ >= capacity - 1) {
				// double the size of the heap
				resize(2 * capacity);
			}
			array[size_++] = value;
			fixHeapFromBelow();
		}

		// forwarding version of insert()
		void insert(val_type&& value) {
			if (size_ >= capacity - 1) {
				// double the size of the heap
				resize(2 * capacity);
			}
			array[size_++] = std::forward<val_type>(value);
			fixHeapFromBelow();
		}

		// variadic template version of insert
		template<class val_type, class... Vals>
		void insert(val_type&& first, Vals&&... values) {
			insert(std::forward<val_type>(first));
			insert(std::forward<Vals>(values)...);
		}

		// private helpers
	private:

		void fixHeapFromAbove() {
			size_t current = 0;

			// while we haven't reached the bottom of the heap
			while (2 * current + 1 < size_) {
				val_type oldValue = array[current];

				if (!fixSingleViolation(current)) {
					break;
				}
				current = getChildIndex(current);
			}
		}

		// gets the index of a child that will be visited next
		size_t getChildIndex(size_t current) {
			size_t leftInd = 2 * current + 1;
			size_t rightInd = 2 * current + 2;

			if (rightInd >= size_) {
				return leftInd;
			}
			else {
				// else return the index of the value with the bigger priority
				// since that is the index of the swapped value
				return compare(array[leftInd], array[rightInd]) ? rightInd : leftInd;
			}
		}

		// restores the heap property by starting from the end of the array
		// returns the index of 
		void fixHeapFromBelow() {

			// size_-1 is the index of the last member of the array
			size_t current = getParentIndex(size_ - 1);

			while (current >= 0) {
				// if the violation is not found, 
				//there is no need to go deeper
				// since there won't be any violations either
				if (!fixSingleViolation(current)) {
					break;
				}
				current = getParentIndex(current);
			}

		}
		// fixes a single violation of the heap property at a given index
		// returns true if violation is fixed, false if there is no violation
		bool fixSingleViolation(size_t current) {

			size_t leftInd = 2 * current + 1;
			size_t rightInd = 2 * current + 2;

			// the index of the smaller (according to their priority value)
			// of the children of current
			size_t extremeInd;
			if (rightInd >= size_) {
				extremeInd = leftInd;
			}
			else {
				extremeInd = compare(array[leftInd], array[rightInd]) ? leftInd : rightInd;
			}

			// if priority of array[current] is less than that of the extreme
			if (compare(array[extremeInd], array[current])) {
				using std::swap;
				swap(array[current], array[extremeInd]);
				return true;
			}
			else {
				// since we are fixing a single violation, there is no need to go higher
				return false;
			}
		}

		// maps a child index to its parent index
		size_t getParentIndex(size_t child) {
			return child % 2 == 0 ? child / 2 - 1 : child / 2;
		}

		// resizes the array according to the new capacity
		void resize(size_t newCapacity) {
			val_type* newArray = new val_type[newCapacity];
			for (unsigned int i = 0; i < size_; ++i) {
				newArray[i] = array[i];
			}
			delete[] array;
			array = newArray;
			capacity = newCapacity;
		}
		// static functions
	private:
		static void fixHeap(val_type* dataArray, int length, int pos, std::function<bool(val_type, val_type)> less = nullptr) {

			while (2 * pos + 1 < length) {
				int newPos;
				int rightPos = 2 * pos + 2;
				int leftPos = 2 * pos + 1;
				int* left = dataArray + leftPos;
				int* right = (rightPos < length) ? dataArray + rightPos : nullptr;

				int* smallerChild;
				if (right == nullptr) {
					smallerChild = left;
					newPos = leftPos;
				}
				else {
					if (less(*left, *right)) {
						smallerChild = left;
						newPos = leftPos;
					}
					else {
						smallerChild = right;
						newPos = rightPos;
					}

				}
				if (less(*smallerChild, dataArray[pos])) {
					using std::swap;
					swap(dataArray[pos], *smallerChild);
				}
				else {
					break;
				}
				pos = newPos;
			}
		}

	public:
		// makes a heap out of an array
		static void heapify(val_type* dataArray, int length, std::function<bool(val_type, val_type)> less = nullptr) {
			if (less == nullptr) {
				// set less to default 
				less = [](const val_type& lhs, const val_type& rhs) -> bool {
					return lhs < rhs;
				};
			}

			for (int i = length / 2; i >= 0; --i) {
				fixHeap(dataArray, length, i, less);
			}
		}

		// sorts an array 
		static void heapSort(val_type* dataArray, int length, std::function<bool(val_type, val_type)> less = nullptr) {
			if (less == nullptr) {
				// set less to default so it will sort in ascending order
				less = [](const val_type& lhs, const val_type& rhs) {
					return lhs > rhs;
				};
			}
			heapify(dataArray, length, less);

			for (int i = length - 1; i > 0; --i) {
				using std::swap;
				swap(dataArray[0], dataArray[i]);
				fixHeap(dataArray, i, 0, less);
			}
		}

	};

}

#endif
