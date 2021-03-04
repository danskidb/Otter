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
		Panic
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

	enum class ECombatAction {
		Melee,
		Gun,
		Skill,
		Consumable,
		Guard,
		_MAX_ENTRY,
	};

	enum class EOpponentType {
		Shadow,
		MinorBoss,	// Increased resistance against all ailments
		MajorBoss,	// Immune to all ailments
	};

	enum class EOdds {
		Low,
		Medium,
		High,
		Guaranteed
	};

	enum class ESkillTargetRestriction {
		None,
		OneFoe,
		AllFoes,
		OneAlly,
		AllAllies,
		Self
	};

	enum class ESkillEffect {
		// Damage
		DamageMinuscule,
		DamageLight,
		DamageMedium,
		DamageHeavy,
		DamageSevere,
		DamageColossal,
		InstaKillSmall,
		InstaKillMedium,
		InstaKillHigh,
		InstaKillFeared,
		DrainSP,
		DrainHP,
		HalveHP,

		// Healing
		HealSlightly,
		HealModerately,
		HealFull,
		ReviveHalf,
		ReviveFull,

		// Support
		IncreaseAttack,
		IncreaseDefense,
		IncreaseAgility,
		IncreaseCritical,
		DecreaseAttack,
		DecreaseDefense,
		DecreaseAgility,
	};

	enum class ESkillEffectModifier {
		PowerUpBatonPass,		// stronger if current turn is a baton pass
		PowerUpAmbush,			// stronger if ECombatStartType == Ambush
		PowerUpEnemyAdvantage,	// stronger if ECombatStartType == EnemyAdvantage
		PowerUpDown,			// stronger if the enemy is downed
		HighCritical,
		HighAccuracy,
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
		std::string description;

		ECombatSkillCostType costType = ECombatSkillCostType::None;
		int cost = 0;

		ESkillTargetRestriction targetRestriction;
		std::vector<ESkillEffect> effects;
		std::vector<ESkillEffectModifier> effectModifiers;
		std::vector<EAilment> ailmentToInflict;			// random chance of one
		EOdds ailmentToInflictOdds;

		int amountAttacksMin = 0;
		int amountAttacksMax = 0;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(CombatSkill, name, description, cost, amountAttacksMin, amountAttacksMax);
	};

	struct Persona {		//todo: lock down this struct so there's a distinction between runtime and static data.
		std::string name;
		Level level = 0;
		EArcana arcana;
		EArcana inherits;

		CombatStat stats;
		std::map<std::string, int> skillUnlocks;
		std::map<EElement, EElementAffinity> elementAffinities;
		std::vector<std::string> activeSkills;

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