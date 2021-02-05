﻿#pragma once
#include <string>
#include <vector>
#include "PersonaData.h"
#include <array>

namespace RpgGame {

	class Compendium
	{
	public:
		void Initialize();

		void ParsePersonaDatabase();
		void ParseSkillDatabase();

		bool FindPersonaById(std::string personaId, Persona &outPersona);
		std::vector<Persona> FindPersonaByArcana(EArcana arcana);

		bool FindSkillById(std::string skillId, CombatSkill &outSkill);
		std::vector<CombatSkill> FindSkillByElement(EElement element);
		
	private:
		const std::string personaJsonPath = "\PersonaData.json";
		const std::string skillsJsonPath = "\Skills.json";
		const std::array<EElement, 10> elemsImportOrder = { EElement::Physical, EElement::Gun, EElement::Fire, EElement::Ice, EElement::Electric, EElement::Wind, EElement::Psychic, EElement::Nuclear, EElement::Bless, EElement::Curse };

		std::map<std::string, Persona> personaCompendium;
		std::map<std::string, CombatSkill> skillCompendium;
	};
}
