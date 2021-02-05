#include "Compendium.h"
#include <nlohmann/json.hpp>
#include <magic_enum.hpp>
#include "Otter/Utils/Filesystem.h"
#include <iostream>
#include <array>
#include <limits>
#include "Otter/Log.h"

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

		for (auto& itr : j.items()) {
			json personaEntry = itr.value();
			Persona persona = personaEntry.get<Persona>();
			persona.name = itr.key();
			persona.item = personaEntry["itemr"].get<std::string>();

			if (personaEntry.contains("rare"))
				persona.rare = personaEntry["rare"].get<bool>();

			if (personaEntry.contains("special"))
				persona.rare = personaEntry["special"].get<bool>();

			// Arcana
			std::string arcanaString = personaEntry["arcana"].get<std::string>();
			auto arcana = magic_enum::enum_cast<EArcana>(arcanaString);
			OT_ASSERT(arcana.has_value(), "Arcana " + arcanaString + " is not defined - found on personaId " + persona.name);
			persona.arcana = arcana.value();

			// Stats
			std::vector<int> statsObj = personaEntry["stats"].get<std::vector<int>>();
			persona.stats = CombatStat { statsObj[0], statsObj[1], statsObj[2], statsObj[3], statsObj[4] };	//todo: can this be cleaner?

			// Element Affinities
			std::vector<std::string> elemsObj = personaEntry["elems"].get<std::vector<std::string>>();
			for (int i = 0; i < elemsImportOrder.size(); i++)
			{
				auto element = magic_enum::enum_cast<EElementAffinity>(elemsObj[i]);
				OT_ASSERT(element.has_value(), "EElementAffinity " + elemsObj[i] + " is not defined - found on personaId " + persona.name + " for element entry" + std::to_string(i));
				persona.elementAffinities.insert({ elemsImportOrder[i], element.value() });
			}

			personaCompendium.insert({ itr.key(), persona });
		}
	}

	bool Compendium::FindPersonaById(std::string personaId, Persona &outPersona)
	{
		if (personaCompendium.size() <= 0)
			return false;

		if (personaCompendium.count(personaId) <= 0)
			return false;

		outPersona = personaCompendium[personaId];
		return true;
	}

	std::vector<Persona> Compendium::FindPersonaByArcana(EArcana arcana)
	{
		std::vector<Persona> result;

		for (auto iter = personaCompendium.begin(); iter != personaCompendium.end(); ++iter)
		{
			if (iter->second.arcana == arcana)
				result.push_back(iter->second);
		}

		return result;
	}
}