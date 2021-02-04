#include "Compendium.h"
#include <nlohmann/json.hpp>
#include <magic_enum.hpp>
#include "Otter/Utils/Filesystem.h"
#include <iostream>
#include <array>
#include <limits>

using json = nlohmann::json;
using namespace Otter;

namespace RpgGame {

	void Compendium::Initialize()
	{
		personaCompendium.clear();

		fs::path absolutePath = Filesystem::GetAssetDirectoryGame();
		absolutePath /= jsonPath;
		fs::canonical(absolutePath);
		std::string fullJsonData = Filesystem::ReadFile(absolutePath);	//todo: see if we can directly get this into a json obj from the file buffer

		json j = json::parse(fullJsonData);
		fullJsonData.clear();

		for (auto& personaEntry : j.items()) {
			Persona persona = personaEntry.value().get<Persona>();
			persona.name = personaEntry.key();

			auto arcana = magic_enum::enum_cast<EArcana>(personaEntry.value()["arcana"].get<std::string>());
			if (arcana.has_value())
				persona.arcana = arcana.value();

			std::vector<int> statsObj = personaEntry.value()["stats"].get<std::vector<int>>();
			persona.stats = CombatStat { statsObj[0], statsObj[1], statsObj[2], statsObj[3], statsObj[4] };	//todo: can this be cleaner?

			//todo: use magic enum
			std::vector<std::string> elemsObj = personaEntry.value()["elems"].get<std::vector<std::string>>();
			for (int i = 0; i < (int)EElement::MAX_ITEMS; i++)
			{
				if (elemsObj[i] == "-")
					persona.elements.insert({ (EElement)i, EElementAffinity::Normal });
				else if (elemsObj[i] == "rs")
					persona.elements.insert({ (EElement)i, EElementAffinity::Resist });
				else if (elemsObj[i] == "wk")
					persona.elements.insert({ (EElement)i, EElementAffinity::Weak });
				else if (elemsObj[i] == "ab")
					persona.elements.insert({ (EElement)i, EElementAffinity::Absorb });
				else if (elemsObj[i] == "rp")
					persona.elements.insert({ (EElement)i, EElementAffinity::Repel });
				else if (elemsObj[i] == "nu")
					persona.elements.insert({ (EElement)i, EElementAffinity::Nullify });
			}

			personaCompendium.insert({ personaEntry.key(), persona });
		}
	}

	bool Compendium::FindPersonaById(std::string personaId, Persona &outPersona)
	{
		if (personaCompendium.size() <= 0)
			return false;

		//todo.

		return false;
	}

}