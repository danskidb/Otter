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

		// Operators
		OString& operator=(const OString& toCopyAssign);
		OString& operator=(OString&& toMoveAssign);

		// Functions
		const char* GetCString();
	};
}

