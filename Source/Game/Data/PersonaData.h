#pragma once
#include <map>
#include <nlohmann/json.hpp>

namespace RpgGame {

	using Level = std::uint8_t;

	enum class EElement{
		Physical,
		Gun,
		Fire,
		Ice,
		Electric,
		Wind,
		Psychic,
		Nuclear,
		Bless,
		Curse,
		Almighty,
		Ailment,
		Support,
		Passive,
		Healing,
		Trait
	};

	enum class EElementAffinity {
		Normal,
		Repel,
		Absorb,
		Resist,
		Weak,
		Nullify
	};

	enum class EAilment {
		Down,
		Burn,
		Freeze,
		Shock,
		Dizzy,
		Forget,
		Sleep,
		Confuse,
		Fear,
		Despair,
		Rage,
		Brainwash,
		Hunger,
		Rattled,
		ArtistsGrace,
		Lust,
		Wrath,
		Envy,
	};

	enum class EArcana {
		Fool,
		Magician,
		Priestess,
		Empress,
		Emperor,
		Hierophant,
		Lovers,
		Chariot,
		Justice,
		Hermit,
		Fortune,
		Strength,
		Hanged,
		Death,
		Temperance,
		Devil,
		Tower,
		Star,
		Moon,
		Sun,
		Judgement,
		Councillor,
		Faith,
		World
	};

	enum class EEquipmentSlot {
		MeleeWeapon,
		RangeWeapon,
		Armor,
		Accessory
	};

	enum class ECombatSkillCostType {
		None,
		Fixed_SP,
		Percentage_HP,
	};

	enum class ECombatStartType {
		Normal,
		Ambush,
		EnemyAdvantage
	};

	struct CombatStat {
		int strength = 0;
		int magic = 0;
		int endurance = 0;
		int agility = 0;
		int luck = 0;
	};

	struct CombatSkill {
		std::string name;
		EElement element;
		std::string effect;

		ECombatSkillCostType costType = ECombatSkillCostType::None;
		int cost = 0;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CombatSkill, effect);
	};

	struct Persona {
		std::string name;
		Level level = 0;
		EArcana arcana;
		EArcana inherits;

		CombatStat stats;
		std::map<std::string, int> skillUnlocks;
		std::map<EElement, EElementAffinity> elementAffinities;

		std::string item;
		std::string trait;

		bool rare = false;
		bool special = false;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Persona, level, trait)
	};

	class Item {
	public:
		std::string name;
		std::string description;

		unsigned int price = 0;
	};

	class Equipment : public Item {
	public:
		EEquipmentSlot slot;

		unsigned int defense = 0;
		unsigned int evasion = 0;

		unsigned int accuracy = 0;
		unsigned int attack = 0;
		unsigned int rounds = 0;
		unsigned int maxRounds = 0;
		//effect - maybe some sort of scripting would be useful here?
		//for now we just go on like this to get basic combat in

		void RefillAmmo() { rounds = maxRounds; }
	};

	struct Character {
		std::string name;
		std::string codename;
		std::string personaId;
		EArcana arcana;
		Level level;

		std::map<EEquipmentSlot, Equipment> allEquipment;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Character, name, level);
	};
}