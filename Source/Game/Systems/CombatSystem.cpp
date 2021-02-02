#include "CombatSystem.h"
#include "Otter/Log.h"

namespace RpgGame {

	void CombatSystem::InitCombat(std::vector<EntityId> characters, std::vector<EntityId> persona)
	{
		OT_ASSERT(!combatOngoing, "Called to initialize combat while it was already ongoing!");
		combatOngoing = true;
	}

	void CombatSystem::Update(float deltaTime)
	{

	}
}


