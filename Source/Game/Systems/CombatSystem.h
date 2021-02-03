#pragma once

#include <vector>
#include "Otter/Core/System.h"
#include "Data/PersonaData.h"

using namespace Otter;

namespace RpgGame {

	/*
		Todo:
		- Add actions players can take
		- critical hits + One More system
	*/

	class CombatSystem : public System {
	public:
		// Starts new combat if not yet started. Verifies inputs, generates turn order and prints it.
		void InitCombat(std::vector<EntityId> _characters, std::vector<EntityId> _opponents, ECombatStartType _combatStartType);

		// Generates a new turn order list and shuffles it. Based on characters and opponents
		void GenerateTurnOrder();

		void OnStartTurn();

		EntityId GetRandomEnemyTarget(EntityId toPickFor);

		void PerformMeleeAttack(EntityId performer, EntityId target);

		void EndTurn();
		
		// Returns true when either all opponents are dead, or all characters are dead
		bool ShouldFinishCombat();

		// Resets the combat system
		void FinishCombat();

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