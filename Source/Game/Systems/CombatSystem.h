#pragma once

#include <vector>
#include "Otter/Core/System.h"

using namespace Otter;

namespace RpgGame {

	class CombatSystem : public System {
	public:
		void InitCombat(std::vector<EntityId> characters, std::vector<EntityId> persona);
		void Update(float deltaTime);

	private:
		bool combatOngoing = false;
	};
}