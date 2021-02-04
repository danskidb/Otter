#pragma once
#include <map>
#include <nlohmann/json.hpp>

namespace RpgGame {

	using PersonaId = std::uint16_t; // todo: remove this
	using Level = std::uint8_t;

	enum class EElement : unsigned int {
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
		MAX_ITEMS,
	};

	enum class EElementAffinity : char {
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
		HangedMan,
		Death,
		Temperance,
		Devil,
		Tower,
		Star,
		Moon,
		Sun,
		Judgement,
	};

	enum class ECombatSkillCostType {
		Fixed_SP,
		Fixed_HP,
		Percentage_HP,
	};

	enum class ECombatStartType {
		Normal,
		Ambush,
		EnemyAdvantage
	};

	struct CombatStat {
		int strength;
		int magic;
		int endurance;
		int agility;
		int luck;
	};

	struct CombatSkill {
		int level;
		int cost;
		std::string name;
		ECombatSkillCostType costType;
		EElement element;
	};

	struct Persona {
		std::string name;
		Level level;
		EArcana arcana;

		CombatStat stats;
		std::vector<CombatSkill> skills;
		std::map<EElement, EElementAffinity> elements;

		std::string item;
		std::string trait;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Persona, level, item, trait)
	};

}