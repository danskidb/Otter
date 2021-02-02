#include "CombatSystem.h"
#include "Otter/Log.h"
#include "Otter/Core/Coordinator.h"

#include "Components/CharacterComponent.h"
#include "Components/PersonaComponent.h"

#include <algorithm>
#include <random>
#include <chrono>

namespace RpgGame {

	void CombatSystem::InitCombat(std::vector<EntityId> _characters, std::vector<EntityId> _opponents, ECombatStartType _combatStartType)
	{
		Coordinator* coordinator = Coordinator::GetInstance();

		OT_ASSERT(!combatOngoing, "Called to initialize combat while it was already ongoing!");
		combatOngoing = true;

		// Verify characters
		OT_ASSERT(_characters.size() <= maxCharacters, "Attempted to start combat with ", _characters.size(), " characters, but the max supported is ", maxCharacters);
		for (EntityId character : _characters)
		{
			OT_ASSERT(coordinator->HasComponent<CharacterComponent>(character), "Character does not have a CharacterComponent!");
		}
		characters = _characters;

		// Verify opponents
		OT_ASSERT(_opponents.size() <= maxOpponents, "Attempted to start combat with ", _opponents.size(), " opponents, but the max supported is ", maxOpponents);
		for (EntityId opponent : _opponents)
		{
			OT_ASSERT(coordinator->HasComponent<PersonaComponent>(opponent), "Opponent does not have a PersonaComponent!");
		}
		opponents = _opponents;

		GenerateTurnOrder();
		OT_INFO("Initialized Combat.");
		DebugPrintState();

	}

	void CombatSystem::GenerateTurnOrder()
	{
		std::vector<EntityId> tempVector;
		tempVector.insert(tempVector.end(), characters.begin(), characters.end());
		tempVector.insert(tempVector.end(), opponents.begin(), opponents.end());

		unsigned seed = std::chrono::system_clock::now()
			.time_since_epoch()
			.count();
		std::shuffle(tempVector.begin(), tempVector.end(), std::default_random_engine(seed));

		turnOrder = tempVector;
	}

	void CombatSystem::DebugPrintState()
	{
		Coordinator* coordinator = Coordinator::GetInstance();
		OT_INFO("Current Turn Order:");

		for (int i = 0; i < turnOrder.size(); i++)
		{
			EntityId entityId = turnOrder[i];
			std::string toPrint;

			if (i == currentTurnSlot)
				toPrint += "> ";
			else
				toPrint += "- ";

			if (coordinator->HasComponent<CharacterComponent>(entityId))
			{
				CharacterComponent character = coordinator->GetComponent<CharacterComponent>(entityId);
				toPrint += "Lv. " + std::to_string(character.level) + " " + character.name;

				EntityId activePersonaEntityId = character.GetActivePersona();
				PersonaComponent persona = coordinator->GetComponent<PersonaComponent>(activePersonaEntityId);
				toPrint += " (Lv. " + std::to_string(persona.level) + " " + persona.name + ")";
			}
			else if (coordinator->HasComponent<PersonaComponent>(entityId))
			{
				PersonaComponent opponent = coordinator->GetComponent<PersonaComponent>(entityId);
				toPrint += "Lv. " + std::to_string(opponent.level) + " " + opponent.name;
			}

			OT_INFO(toPrint);
		}

	}

}


