#pragma once
#include <iterator>
#include <cstddef>

namespace Otter {

	template<typename T, size_t L>
	class OArray
	{
	public:
		//todo: constructor for which we can define the array contents: OArray<OString, 2> array = {OString(), OString()}

		T& operator[](size_t index) { return content[index]; }
		const T& operator[](size_t index) const { return content[index]; }	// make both the ptr and value const

		const int Length() { return L; }

	private:
		T content[L];

	public:
		//todo: const iterator
		struct Iterator {	// todo: support reverse iterators
			// https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp

			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = value_type*;
			using reference = value_type&;

			Iterator(pointer _ptr) : ptr(_ptr) {}

			reference operator*() const { return *ptr; }
			pointer operator->() { return ptr; }
			Iterator& operator++() { ptr++; return *this; }								//Prefix increment
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }	//Postfix increment
			//todo: operator-
			friend bool operator== (const Iterator& a, const Iterator& b) { return a.ptr == b.ptr; };
			friend bool operator!= (const Iterator& a, const Iterator& b) { return a.ptr != b.ptr; };

		private:
			pointer ptr;
		};

		Iterator begin() { return Iterator(&content[0]); }
		Iterator end() { return Iterator(&content[L]); }
	};
}