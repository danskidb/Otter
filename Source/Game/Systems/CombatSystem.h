#pragma once

#include <vector>
#include "Otter/Core/System.h"
#include "Data/PersonaData.h"

using namespace Otter;

namespace RpgGame {

	class CombatSystem : public System {
	public:
		void InitCombat(std::vector<EntityId> characters, std::vector<EntityId> opponents, ECombatStartType combatStartType);
		void Update(float deltaTime);

	private:
		bool combatOngoing = false;

		std::vector<EntityId> characters;	//These all have CharacterComponent
		std::vector<EntityId> opponents;	//These all have PersonaComponent
		std::vector<EntityId> turnOrder;
	};
}