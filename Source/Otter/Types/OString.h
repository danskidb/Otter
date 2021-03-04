#pragma once

namespace Otter
{
	/*
		Custom String Implementation
	*/
	class OString
	{
	private:
		char* content;
		size_t contentSize;	//without null-char

	public:
		// constructors
		OString();
		OString(const char* toCopy);
		OString(const OString& toCopy);
		OString(OString&& toMove);
		~OString();

	protected:
		OString(size_t sizeToReserve);

	public:
		// Operators
		OString& operator=(const OString& toCopyAssign);
		OString& operator=(OString&& toMoveAssign);

		OString operator+(const OString& toAppend);
		OString operator+(const char* toAppend);
		OString& operator+=(const OString& toAppend);
		OString& operator+=(const char* toAppend);

		// Functions
		const char* GetCString();
	};
}

