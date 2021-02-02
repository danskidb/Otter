#pragma once

#include <vector>
#include "Otter/Core/System.h"
#include "Data/PersonaData.h"

using namespace Otter;

namespace RpgGame {

	class CombatSystem : public System {
	public:
		void InitCombat(std::vector<EntityId> _characters, std::vector<EntityId> _opponents, ECombatStartType _combatStartType);
		void GenerateTurnOrder();
		void DebugPrintState();

	private:
		const unsigned int maxCharacters = 4;
		const unsigned int maxOpponents = 4;

		bool combatOngoing = false;

		std::vector<EntityId> characters;	//These all have CharacterComponent
		std::vector<EntityId> opponents;	//These all have PersonaComponent

		std::vector<EntityId> turnOrder;
		const unsigned int currentTurnSlot = 0;
	};
}