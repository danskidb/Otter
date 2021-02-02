#pragma once

#include <vector>
#include "Data/PersonaData.h"
#include <Otter.h>

namespace RpgGame {

	struct CharacterComponent {
		std::string name;
		Level level;
		EArcana arcana;

		std::vector<PersonaId> ownedPersona;
		std::vector<Otter::EntityId> instantiatedPersona;

		CharacterComponent() {}
		CharacterComponent(std::string _name, Level _level, EArcana _arcana) : name(_name), level(_level), arcana(_arcana) {}
	};
}