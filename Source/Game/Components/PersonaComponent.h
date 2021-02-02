#pragma once

#include "Data/PersonaData.h"
#include <vector>
#include <map>

namespace RpgGame {

	struct PersonaComponent {
		PersonaId personaId;
		std::string name;
		Level level;
		EArcana arcana;

		CombatStat stats;
		std::vector<CombatSkill> skills;	// skill unlock at?
		std::map<EElement, EElementAffinity> elements;

		std::string item;
		std::string trait;

		PersonaComponent() {}
		PersonaComponent(PersonaId _personaId, std::string _name, Level _level, EArcana _arcana) 
			: personaId(_personaId), name(_name), level(_level), arcana(_arcana) {}
	};
}