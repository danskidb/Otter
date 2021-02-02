#pragma once

#include <vector>
#include "Data/PersonaData.h"
#include "Otter/Core/Types.h"

namespace RpgGame {
	
	using namespace Otter;

	class CharacterComponent {
	public:
		std::string name;
		Level level;
		EArcana arcana;
		std::vector<PersonaId> ownedPersona;	// Most have one persona but this should also work for the MC who can switch between personas.

		// Live data
		std::vector<EntityId> instantiatedPersona;
		int activePersona = -1;

		CharacterComponent() {}
		CharacterComponent(std::string _name, Level _level, EArcana _arcana) : name(_name), level(_level), arcana(_arcana) {}

		void AddPersona(EntityId personaEntityId);
		void ActivatePersona(unsigned int toActivate);
		EntityId GetActivePersona();
	};
}