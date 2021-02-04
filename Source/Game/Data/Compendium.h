#pragma once
#include <string>
#include <vector>
#include "PersonaData.h"

namespace RpgGame {

	class Compendium
	{
	public:
		void Initialize();
		bool FindPersonaById(std::string personaId, Persona &outPersona);
		
	private:
		const std::string jsonPath = "\PersonaData.json";
		std::map<std::string, Persona> personaCompendium;
	};
}

