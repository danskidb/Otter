#pragma once
#include <string>
#include <vector>
#include "PersonaData.h"
#include <array>

namespace RpgGame {

	class Compendium
	{
	public:
		void Initialize();
		bool FindPersonaById(std::string personaId, Persona &outPersona);
		std::vector<Persona> FindPersonaByArcana(EArcana arcana);
		
	private:
		const std::string jsonPath = "\PersonaData.json";
		std::map<std::string, Persona> personaCompendium;
		std::array<EElement, 10> elemsImportOrder = { EElement::Physical, EElement::Gun, EElement::Fire, EElement::Ice, EElement::Electric, EElement::Wind, EElement::Psychic, EElement::Nuclear, EElement::Bless, EElement::Curse };
	};
}

