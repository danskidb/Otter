#include "OString.h"
#include <stdio.h>
#include <string.h>
#include <type_traits>

namespace Otter
{
	OString::OString()
	{
		// we init a new char array with length 1, and a terminating null-character.
		// The null/empty char is simply a value of zero, but can also be represented as a character with an escaped zero.
		content = new char[1];
		content[0] = '\0';		// end of char signifier
		contentSize = 0;
	}

	OString::OString(const char* toCopy)
	{
		if (toCopy == nullptr)
		{
			content = new char[1];
			content[0] = '\0';
			contentSize = 0;
			printf("Created empty OString\n");
		}
		else
		{
			// copy contents
			contentSize = strlen(toCopy);
			content = new char[contentSize + 1];	// leave room for \0
			memcpy(content, toCopy, contentSize + 1);
			printf("Created OString from char %s\n", toCopy);
		}
	}

	OString::OString(const OString& toCopy)
	{
		contentSize = toCopy.contentSize;
		content = new char[contentSize + 1];	// leave room for \0
		memcpy(content, toCopy.content, contentSize + 1);
		printf("Copied OString %s\n", content);
	}

	OString::OString(OString&& toMove) noexcept
	{
		contentSize = std::move(toMove.contentSize);
		content = std::move(toMove.content);

		toMove.content = nullptr;
		toMove.contentSize = 0;
		printf("Moved OString %s\n", content);
	}

	OString::~OString()
	{
		printf("Deleting OString with content %s\n", content);
		delete[] content;
	}

	OString::OString(size_t sizeToReserve)
	{
		content = new char[sizeToReserve + 1];
		content[sizeToReserve] = '\0';
		contentSize = sizeToReserve;
		printf("Created empty OString of size %zu\n", sizeToReserve);
	}

	OString& OString::operator=(const OString& toCopyAssign)
	{
		if (this != &toCopyAssign)
		{
			delete[] content;

			contentSize = toCopyAssign.contentSize;
			content = new char[contentSize + 1];	// leave room for \0
			memcpy(content, toCopyAssign.content, contentSize + 1);
			printf("CopyAssigned OString\n");
		}

		return *this;
	}

	OString& OString::operator=(OString&& toMoveAssign)
	{
		if (this != &toMoveAssign)
		{
			delete[] content;

			contentSize = std::move(toMoveAssign.contentSize);
			content = std::move(toMoveAssign.content);

			toMoveAssign.content = nullptr;
			toMoveAssign.contentSize = 0;
			printf("MoveAssigned OString\n");
		}

		return *this;
	}

	OString OString::operator+(const OString& toAppend)
	{
		OString result(contentSize + toAppend.contentSize);
		memcpy(result.content, content, contentSize);										// todo: for all + and += operators, make a generic move/copy func
		memcpy(result.content + contentSize, toAppend.content, toAppend.contentSize + 1);	// increment pointer location to the end of the previous memcopy

		return result;
	}

	OString OString::operator+(const char* toAppend)
	{
		size_t appendSize = strlen(toAppend);
		OString result(contentSize + appendSize);
		memcpy(result.content, content, contentSize);
		memcpy(result.content + contentSize, toAppend, appendSize + 1);

		return result;
	}

	OString& OString::operator+=(const OString& toAppend)
	{
		size_t newSize = contentSize + toAppend.contentSize;
		char* newContent = new char[newSize + 1];
		memcpy(newContent, content, contentSize);
		memcpy(newContent + contentSize, toAppend.content, toAppend.contentSize + 1);

		delete[] content;
		contentSize = newSize;
		content = newContent;

		return *this;
	}

	OString& OString::operator+=(const char* toAppend)
	{
		size_t appendSize = strlen(toAppend);
		size_t newSize = contentSize + appendSize;
		char* newContent = new char[newSize + 1];
		memcpy(newContent, content, contentSize);
		memcpy(newContent + contentSize, toAppend, appendSize + 1);

		delete[] content;
		contentSize = newSize;
		content = newContent;

		return *this;
	}

	const char* OString::GetCString()
	{
		return content;
	}

	const size_t OString::Length()
	{
		return contentSize;
	}

}