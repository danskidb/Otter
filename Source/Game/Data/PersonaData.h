#pragma once
#include <map>
#include <nlohmann/json.hpp>

namespace RpgGame {

	using PersonaId = std::uint16_t; // todo: remove this
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
		Level level;
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
}