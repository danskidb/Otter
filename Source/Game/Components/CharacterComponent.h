#pragma once

#include <vector>
#include "Data/PersonaData.h"
#include "Otter/Core/Types.h"

using namespace Otter;

namespace RpgGame {
	
	struct CharacterComponent {
		std::string name;
		Level level;
		EArcana arcana;
		std::vector<std::string> ownedPersona;	// Most have one persona but this should also work for the MC who can switch between personas.

		CharacterComponent() {}
		CharacterComponent(std::string _name, Level _level, EArcana _arcana) : name(_name), level(_level), arcana(_arcana) {}
	};
}