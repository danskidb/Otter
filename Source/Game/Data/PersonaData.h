#pragma once

namespace RpgGame {

	using PersonaId = std::uint16_t;
	using Level = std::uint8_t;

	enum EElement {
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

	enum EElementAffinity {
		Normal,
		Reflect,
		Absorb,
		Block,
		Resist,
		Weak
	};

	enum EAilment {
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

	enum EArcana {
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

	enum ECombatSkillCostType {
		Fixed_SP,
		Fixed_HP,
		Percentage_HP,
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