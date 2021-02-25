#pragma once

#include <vector>
#include "Otter/Core/System.h"
#include "Data/PersonaData.h"

using namespace Otter;

namespace RpgGame {

	/*
		//TODO:
		Looks like we'll want some sort of Temporary arrays with duration for player element affinities

		- Multiply user’s next Phys attack damage by 2.5.
		- Multiply user’s next magical attack damage by 2.5.
		- Wall barriers
		- Absorbs all non-almighty attacks once and removes weakness.
		- Repels incoming non-almighty attack for all allies.
		- Applies Attack Master and Concentrate on one ally.

		- https://megamitensei.fandom.com/wiki/Damage
		- skills
		- Technical
		- ailment: https://samurai-gamers.com/persona-5/list-conditions-ailments-persona-5-p5/
		- https://samurai-gamers.com/persona-5/support-skills-list/
		- https://samurai-gamers.com/persona-5/healing-skills-list/
		- https://samurai-gamers.com/persona-5/status-recovery-skills-list/
		- https://samurai-gamers.com/persona-5/passive-skills-list/
		- https://samurai-gamers.com/persona-5/status-ailments-list/
		- Modifiers:
			* effective
			* buffs/debuffs
			* passive skills
			* end damage variance
			* level difference multipliers
	*/

	class CombatSystem : public System {
	public:
		virtual void OnRegistered() override {}

		// Starts new combat if not yet started. Verifies inputs, generates turn order and prints it.
		void InitCombat(std::vector<EntityId> _characters, std::vector<EntityId> _opponents, ECombatStartType _combatStartType);

		// Generates a new turn order list and shuffles it. Based on characters and opponents
		void GenerateTurnOrder();

		void OnStartTurn(bool isRestart = false);

		EntityId GetRandomEnemyTarget(EntityId toPickFor);

		std::vector<EntityId> FilterAliveEntities(std::vector<EntityId> characterOrOpponentVector);

		// attack power for a gun or melee weapon
		float GetCharacterAttackPower(Equipment* meleeWeapon, CombatStat* attackerStats);

		float SubtractDefense(float baseAttackDamage, CombatStat* defendantStats, Equipment* defendantArmor);

		void PerformMeleeAttack(EntityId performer, EntityId target);

		void PerformRangedAttack(EntityId performer, EntityId target);

		void PerformSkill(EntityId performer, EntityId target);

		void EndTurn();
		
		// Returns true when either all opponents are dead, or all characters are dead
		bool ShouldFinishCombat();

		// Resets the combat system
		void FinishCombat();

		// Asks the player for a supported action.
		ECombatAction AskPlayerAction();

		// Asks the player to select a target from the turn order that's alive.
		// Todo: handle filtering, like including dead characters or only opponents.
		// Todo: Implement cancelling and going back to AskPlayerAction.
		EntityId AskPlayerTarget();

		// Loops through turn order and prints lv, name, etc to console.
		void DebugPrintState();

	private:
		const unsigned int maxCharacters = 4;
		const unsigned int maxOpponents = 4;

		bool combatOngoing = false;

		std::vector<EntityId> characters;	//These all have CharacterComponent
		std::vector<EntityId> opponents;	//These all have PersonaComponent
		bool IsCharacter(EntityId entityId);
		std::string GetName(EntityId entityId);
		Level GetLevel(EntityId entityId);

		std::vector<EntityId> turnOrder;
		unsigned int currentTurn = 0;
		unsigned int currentRound = 1;
	};
}