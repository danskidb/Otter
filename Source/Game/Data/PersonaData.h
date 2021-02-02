#pragma once

namespace RpgGame {

	using PersonaId = std::uint16_t;
	using Level = std::uint8_t;

	enum class EElement {
		Physical,
		Gun,
		Fire,
		Ice,
		Electric,
		Wind,
		Psy,
		Nuclear,
		Bless,
		Curse,
		Almighty,
		Ailment,
		Support,
		Passive,
		Healing,
	};

	enum class EElementAffinity {
		Normal,
		Reflect,
		Absorb,
		Block,
		Resist,
		Weak
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

}