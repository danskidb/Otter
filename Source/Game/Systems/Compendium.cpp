#include "Compendium.h"
#include <nlohmann/json.hpp>
#include <magic_enum.hpp>
#include <iostream>
#include <array>
#include <limits>
#include "Otter/Utils/Log.h"

using json = nlohmann::json;
using namespace Otter;

namespace RpgGame {

	void Compendium::OnRegistered()
	{
		skillCompendium.clear();
		personaCompendium.clear();
		equipmentCompendium.clear();
		characterCompendium.clear();

		ParseSkillDatabase();
		ParsePersonaDatabase();
		ParseEquipmentDatabase();
		ParseCharacterDatabase();
	}

	void Compendium::ParsePersonaDatabase()
	{
		fs::path absolutePath = Filesystem::GetAssetDirectoryGame();
		absolutePath /= personaJsonPath;
		std::string fullJsonData = Filesystem::ReadFile(absolutePath);	//todo: see if we can directly get this into a json obj from the file buffer

		// Import persona database
		json j = json::parse(fullJsonData);
		fullJsonData.clear();
		
		for (auto& itr : j.items()) 
		{
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

			// Skills
			if (personaEntry.contains("skills"))
			{
				json skillTable = personaEntry["skills"];
				for (auto& skillUnlocks : skillTable.items())
				{
					// Not all skills are implemented, so we filter them out of the runtime.
					CombatSkill c;
					if (!FindSkillById(skillUnlocks.key(), c))
						continue;

					// add to list of all unlocks
					persona.skillUnlocks.insert({ skillUnlocks.key(), skillUnlocks.value() });

					// and lastly, to the skills the persona can actually use with their current level.
					if (skillUnlocks.value() <= skillUnlocks.value())
						persona.activeSkills.push_back({ skillUnlocks.key() });
				}
			}

			personaCompendium.insert({ itr.key(), persona });
		}
	}

	void Compendium::ParseSkillDatabase()
	{
		fs::path absolutePath = Filesystem::GetAssetDirectoryGame();
		absolutePath /= skillsJsonPath;
		std::string fullJsonData = Filesystem::ReadFile(absolutePath);

		// Import skill database
		json j = json::parse(fullJsonData);
		fullJsonData.clear();

		for (auto& itr : j.items()) 
		{
			json skillEntry = itr.value();
			CombatSkill combatSkill = skillEntry.get<CombatSkill>();

			// element
			std::string elementString = skillEntry["element"].get<std::string>();
			auto element = magic_enum::enum_cast<EElement>(elementString);
			OT_ASSERT(element.has_value(), "Element " + elementString + " is not defined - found on skillId " + combatSkill.name);
			combatSkill.element = element.value();

			// element
			std::string costTypeString = skillEntry["costType"].get<std::string>();
			auto costType = magic_enum::enum_cast<ECombatSkillCostType>(costTypeString);
			OT_ASSERT(costType.has_value(), "CostType " + costTypeString + " is not defined - found on skillId " + combatSkill.name);
			combatSkill.costType = costType.value();

			// TargetRestriction
			std::string targetRestrictionString = skillEntry["targetRestriction"].get<std::string>();
			auto targetRestriction = magic_enum::enum_cast<ESkillTargetRestriction>(targetRestrictionString);
			OT_ASSERT(targetRestriction.has_value(), "targetRestriction " + targetRestrictionString + " is not defined - found on skillId " + combatSkill.name);
			combatSkill.targetRestriction = targetRestriction.value();

			// effects (optional)
			if (skillEntry.contains("effects"))
			{
				json effectJsonArray = skillEntry["effects"];
				for (auto& effectItr : effectJsonArray.items())
				{
					std::string effectString = effectItr.value();
					auto effect = magic_enum::enum_cast<ESkillEffect>(effectString);
					OT_ASSERT(effect.has_value(), "effect " + effectString + " is not defined - found on skillId " + combatSkill.name);
					combatSkill.effects.push_back(effect.value());
				}
			}

			// effect modifiers (optional)
			if (skillEntry.contains("effectModifiers"))
			{
				json effectModifierJsonArray = skillEntry["effectModifiers"];
				for (auto& effectModifierItr : effectModifierJsonArray.items())
				{
					std::string effectModifierString = effectModifierItr.value();
					auto effectModifier = magic_enum::enum_cast<ESkillEffectModifier>(effectModifierString);
					OT_ASSERT(effectModifier.has_value(), "effectModifier " + effectModifierString + " is not defined - found on skillId " + combatSkill.name);
					combatSkill.effectModifiers.push_back(effectModifier.value());
				}
			}

			// ailments to inflict (optional)
			if (skillEntry.contains("ailmentToInflict"))
			{
				json ailmentToInflictJsonArray = skillEntry["ailmentToInflict"];
				for (auto& ailmentToInflictItr : ailmentToInflictJsonArray.items())
				{
					std::string ailmentToInflictString = ailmentToInflictItr.value();
					auto ailment = magic_enum::enum_cast<EAilment>(ailmentToInflictString);
					OT_ASSERT(ailment.has_value(), "ailment " + ailmentToInflictString + " is not defined - found on skillId " + combatSkill.name);
					combatSkill.ailmentToInflict.push_back(ailment.value());
				}
			}

			// ailmentToInflictOdds
			if (skillEntry.contains("ailmentToInflictOdds"))
			{
				std::string ailmentToInflictOddsString = skillEntry["ailmentToInflictOdds"].get<std::string>();
				auto ailmentToInflictOdds = magic_enum::enum_cast<EOdds>(ailmentToInflictOddsString);
				OT_ASSERT(ailmentToInflictOdds.has_value(), "ailmentToInflictOdds " + ailmentToInflictOddsString + " is not defined - found on skillId " + combatSkill.name);
				combatSkill.ailmentToInflictOdds = ailmentToInflictOdds.value();
			}

			skillCompendium.insert({ itr.key(), combatSkill });
		}
	}

	void Compendium::ParseEquipmentDatabase()
	{
		fs::path absolutePath = Filesystem::GetAssetDirectoryGame();
		absolutePath /= equipmentJsonPath;
		std::string fullJsonData = Filesystem::ReadFile(absolutePath);

		json j = json::parse(fullJsonData);
		fullJsonData.clear();

		for (auto& itr : j.items())
		{
			auto equipmentSlot = magic_enum::enum_cast<EEquipmentSlot>(itr.key());
			OT_ASSERT(equipmentSlot.has_value(), "Equipment slot " + itr.key() + " is not defined");

			json equipmentlist = itr.value();
			for (auto& equipmentSlotItr : equipmentlist.items())
			{
				json equipmentEntry = equipmentSlotItr.value();

				Equipment equipment;
				equipment.name = equipmentSlotItr.key();
				equipment.slot = equipmentSlot.value();

				// todo: this is dumb, create macro that can NLOHMANN_DEFINE_TYPE_INTRUSIVE but with items that dont exist.
				if (equipmentEntry.contains("defense"))
					equipment.defense = equipmentEntry["defense"].get<unsigned int>();
				if (equipmentEntry.contains("evasion"))
					equipment.evasion = equipmentEntry["evasion"].get<unsigned int>();
				if (equipmentEntry.contains("accuracy"))
					equipment.accuracy = equipmentEntry["accuracy"].get<unsigned int>();
				if (equipmentEntry.contains("attack"))
					equipment.attack = equipmentEntry["attack"].get<unsigned int>();
				if (equipmentEntry.contains("rounds"))
					equipment.maxRounds = equipmentEntry["rounds"].get<unsigned int>();
				if (equipmentEntry.contains("description"))
					equipment.description = equipmentEntry["description"].get<std::string>();

				equipmentCompendium.insert({ equipmentSlotItr.key(), equipment });
			}
		}
	}

	void Compendium::ParseCharacterDatabase()
	{
		fs::path absolutePath = Filesystem::GetAssetDirectoryGame();
		absolutePath /= characterJsonPath;
		std::string fullJsonData = Filesystem::ReadFile(absolutePath);

		json j = json::parse(fullJsonData);
		fullJsonData.clear();

		for (auto& itr : j.items())
		{
			json characterEntry = itr.value();
			Character character = characterEntry.get<Character>();
			character.codename = itr.key();

			// arcana
			std::string arcanaString = characterEntry["arcana"].get<std::string>();
			auto arcana = magic_enum::enum_cast<EArcana>(arcanaString);
			OT_ASSERT(arcana.has_value(), "Arcana " + arcanaString + " is not defined - found on characterId " + character.codename);
			character.arcana = arcana.value();

			if (characterEntry.contains("persona"))
				character.personaId = characterEntry["persona"].get<std::string>();

			json defaultEquipmentJson = characterEntry["defaultEquipment"];
			for (auto& equipmentItr : defaultEquipmentJson.items())
			{
				if (equipmentItr.value() == "")
					continue;

				auto equipmentSlot = magic_enum::enum_cast<EEquipmentSlot>(equipmentItr.key());
				OT_ASSERT(equipmentSlot.has_value(), "Equipment slot " + equipmentItr.key() + " is not defined");

				Equipment found;
				bool result = FindEquipmentById(equipmentItr.value(), found);
				if (result)
					character.allEquipment.insert({ equipmentSlot.value(), found });
			}

			characterCompendium.insert({ itr.key(), character });
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

	bool Compendium::FindSkillById(std::string skillId, CombatSkill &outSkill)
	{
		if (skillCompendium.size() <= 0)
			return false;

		if (skillCompendium.count(skillId) <= 0)
			return false;

		outSkill = skillCompendium[skillId];
		return true;
	}

	std::vector<CombatSkill> Compendium::FindSkillsById(std::vector<std::string> skillIds)
	{
		std::vector<CombatSkill> result;
		for (std::string skillId : skillIds)
		{
			CombatSkill c;
			if (FindSkillById(skillId, c))
				result.push_back(c);
		}

		return result;
	}

	std::vector<CombatSkill> Compendium::FindSkillByElement(EElement element)
	{
		std::vector<CombatSkill> result;

		for (auto iter = skillCompendium.begin(); iter != skillCompendium.end(); ++iter)
		{
			if (iter->second.element == element)
				result.push_back(iter->second);
		}

		return result;
	}

	bool Compendium::FindEquipmentById(std::string equipmentId, Equipment &outEquipment)
	{
		if (equipmentCompendium.size() <= 0)
			return false;

		if (equipmentCompendium.count(equipmentId) <= 0)
			return false;

		outEquipment = equipmentCompendium[equipmentId];
		return true;
	}

	bool Compendium::FindCharacterById(std::string characterId, Character &outCharacter)
	{
		if (characterCompendium.size() <= 0)
			return false;

		if (characterCompendium.count(characterId) <= 0)
			return false;

		outCharacter = characterCompendium[characterId];
		return true;
	}
}