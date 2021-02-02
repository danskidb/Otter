#pragma once

#include <vector>
#include "Data/PersonaData.h"
#include <Otter.h>

namespace RpgGame {

	struct CharacterComponent {
		std::string name;
		Level level;
		EArcana arcana;
		std::vector<PersonaId> ownedPersona;	// Most have one persona but this should also work for the MC who can switch between personas.

		// Live data
		std::vector<Otter::EntityId> instantiatedPersona;
		int activatePersona = -1;	//todo: change from struct to class and add a function to add / activate personas with verification.

		CharacterComponent() {}
		CharacterComponent(std::string _name, Level _level, EArcana _arcana) : name(_name), level(_level), arcana(_arcana) {}
	};
}