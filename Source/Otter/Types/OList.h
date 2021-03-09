#pragma once
#include "Otter/Utils/Log.h"

namespace Otter {

	/*
		Custom dynamic array implementation
	*/
	template<typename T>
	class OList
	{
	public:
		OList() = default;
		//OList(const OList& toCopy) = delete;					// To define
		//OList& operator=(const Olist& toCopyAssign) = delete;	// To define
		~OList() 
		{
			delete[] content;
			content = nullptr;
		}

		T& operator[](size_t index)
		{ 
			OT_ASSERT(index < length, "Attempted to get object out of bounds.");
			return content[index]; 
		}

		const T& operator[](size_t index) const
		{
			OT_ASSERT(index < length, "Attempted to get object out of bounds.");
			return content[index];
		}

		inline void Push(const T& toAppend)
		{
			if (length >= allocated)
				Reallocate(allocated + 1);
			
			content[length] = toAppend;
			length++;
		}

		inline void Push(T&& toAppend)
		{
			if (length >= allocated)
				Reallocate(allocated + 1);

			content[length] = std::move(toAppend);
			length++;
		}

		// Variadric template
		template<typename... Args>
		inline T& Emplace(Args&&... args)
		{
			if (length >= allocated)
				Reallocate(allocated + 1);

			// calling 'new(ptr) Type()' expands to 'new(sizeof(Type), ptr) Type()'. We force it to go to a memory address we already know.
			// source: https://youtu.be/ryRf4Jh_YC0?t=1974, more info: https://www.geeksforgeeks.org/placement-new-operator-cpp/
			// we could leave out the lvalue since unused but this should make the syntax a bit more recognisable.
			// Alternatively you can write "content[length] = T(std::forward<Args>(args)...);" but that makes the function just a glorified Pushback, since you end up calling the move constructor anyways.
			T* emplaced = new(&content[length]) T(std::forward<Args>(args)...);
			return content[length - 1];
		}

		inline void Reserve(size_t newLength) { Reallocate(newLength); }
		inline const bool HasUnusedCapacity() { return length < allocated; }
		inline const bool IsEmpty() { return length <= 0; }
		inline const size_t Length() { return length; }

	private:
		inline void Reallocate(const size_t& newAllocatedLength)
		{
			OT_ASSERT(newAllocatedLength > length, "Reallocating list to memory space smaller than current length.");

			T* newAllocated = new T[newAllocatedLength];

			// instead of calling memcpy, ensure deep copy happens by calling the individual copy constructors 
			for (size_t i = 0; i < length; i++)
				newAllocated[i] = std::move(content[i]); // falls back to copy if no move constructor.

			delete[] content;
			content = newAllocated;
			allocated = newAllocatedLength;
		}

		size_t allocated = 0;
		size_t length = 0;
		T* content = nullptr;
	};
}
