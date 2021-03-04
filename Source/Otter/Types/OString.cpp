#include "OString.h"
#include <stdio.h>
#include <string.h>

namespace Otter
{
	OString::OString()
	{
		// we init a new char array with length 1, and a terminating null-character.
		// The null/empty char is simply a value of zero, but can also be represented as a character with an escaped zero.
		content = new char[1];
		content[0] = '\0';		// end of char signifier
		contentSize = 0;
		printf("Created empty OString\n");
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
		printf("Copied OString\n");
	}

	OString::OString(OString&& toMove)
	{
		contentSize = toMove.contentSize;
		content = toMove.content;

		toMove.content = nullptr;
		toMove.contentSize = 0;
		printf("Moved OString\n");
	}

	OString::~OString()
	{
		printf("Deleting OString with content %s\n", content);
		delete content;
	}

	OString& OString::operator=(const OString& toCopyAssign)
	{
		if (this != &toCopyAssign)
		{
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
			contentSize = toMoveAssign.contentSize;
			content = toMoveAssign.content;

			toMoveAssign.content = nullptr;
			toMoveAssign.contentSize = 0;
			printf("MoveAssigned OString\n");
		}

		return *this;
	}

	const char* OString::GetCString()
	{
		return content;
	}
}