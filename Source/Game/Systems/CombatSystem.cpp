#include "CombatSystem.h"
#include "Otter/Log.h"
//#include "Components/CharacterComponent.h"

namespace RpgGame {

	void CombatSystem::InitCombat(std::vector<EntityId> characters, std::vector<EntityId> opponents, ECombatStartType combatStartType)
	{
		OT_ASSERT(!combatOngoing, "Called to initialize combat while it was already ongoing!");
		combatOngoing = true;

		// Verify that characters all have CharacterComponent
		for (EntityId character : characters)
		{
			//OT_ASSERT(coordinator->HasComponent<CharacterComponent>(character), "Character does not have a CharacterComponent!");
		}

		// Verify that opponents all have PersonaComponent
		for (EntityId opponent : opponents)
		{
			//OT_ASSERT(coordinator->HasComponent<CharacterComponent>(opponent), "Opponent does not have a PersonaComponent!");
		}
	}

	void CombatSystem::Update(float deltaTime)
	{

	}
}


