#pragma once
#include <string>
#include <vector>
#include "Data/PersonaData.h"
#include <array>
#include "Otter/Utils/Filesystem.h"
#include "Otter/Core/System.h"

using namespace Otter;

namespace RpgGame {

	class Compendium : public System
	{
	public:
		virtual void OnRegistered() override;

		void ParsePersonaDatabase();
		void ParseSkillDatabase();
		void ParseEquipmentDatabase();
		void ParseCharacterDatabase();

		bool FindPersonaById(std::string personaId, Persona &outPersona);
		std::vector<Persona> FindPersonaByArcana(EArcana arcana);

		bool FindSkillById(std::string skillId, CombatSkill &outSkill);
		std::vector<CombatSkill> FindSkillsById(std::vector<std::string> skillIds);
		std::vector<CombatSkill> FindSkillByElement(EElement element);

		bool FindEquipmentById(std::string equipmentId, Equipment &outEquipment);
		bool FindCharacterById(std::string characterId, Character &outCharacter);

	private:
		const std::string personaJsonPath = "PersonaData.json";
		const std::string skillsJsonPath = "skills.json";
		const std::string characterJsonPath = "Characters.json";
		const std::string equipmentJsonPath = "Equipment.json";
		const std::array<EElement, 10> elemsImportOrder = { EElement::Physical, EElement::Gun, EElement::Fire, EElement::Ice, EElement::Electric, EElement::Wind, EElement::Psychic, EElement::Nuclear, EElement::Bless, EElement::Curse };

		std::map<std::string, Persona> personaCompendium;
		std::map<std::string, CombatSkill> skillCompendium;
		std::map<std::string, Equipment> equipmentCompendium;
		std::map<std::string, Character> characterCompendium;
	};
}