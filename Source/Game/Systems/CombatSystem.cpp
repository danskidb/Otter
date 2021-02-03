#include "CombatSystem.h"
#include "Otter/Log.h"
#include "Otter/Core/Coordinator.h"

#include "Components/CharacterComponent.h"
#include "Components/PersonaComponent.h"
#include "Components/CombatComponent.h"

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
		OnStartTurn();
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

	void CombatSystem::OnStartTurn()
	{
		Coordinator* coordinator = Coordinator::GetInstance();
		EntityId entity = turnOrder[currentTurn];

		if (currentTurn == 0)
			DebugPrintState();

		if (IsCharacter(entity)) 
		{
			//todo: give player choice
		}
		else
		{
			//todo: process AI turn
		}

		//debug:
		EntityId target = GetRandomEnemyTarget(entity);
		PerformMeleeAttack(entity, target);
	}

	EntityId CombatSystem::GetRandomEnemyTarget(EntityId toPickFor)
	{
		unsigned seed = std::chrono::system_clock::now()
			.time_since_epoch()
			.count();
		std::default_random_engine generator(seed);

		if (IsCharacter(toPickFor))
		{
			std::uniform_int_distribution<int> distribution(0, opponents.size() - 1);
			return opponents[distribution(generator)];
		}
		else
		{
			std::uniform_int_distribution<int> distribution(0, characters.size() - 1);
			return characters[distribution(generator)];
		}
	}

	void CombatSystem::PerformMeleeAttack(EntityId performer, EntityId target)
	{
		Coordinator* coordinator = Coordinator::GetInstance();
		CombatComponent& targetCombatComponent = coordinator->GetComponent<CombatComponent>(target);

		//temporary, attack character for <level> damage
		int dmg = GetLevel(performer);
		targetCombatComponent.hp -= dmg;
		OT_INFO(GetName(performer) + " attacked " + GetName(target) + " with Melee for " + std::to_string(dmg) + " damage.");

		if (!targetCombatComponent.IsAlive())
			OT_INFO(GetName(target) + " fainted.");

		EndTurn();
	}

	void CombatSystem::EndTurn()
	{
		if (ShouldFinishCombat())
		{
			FinishCombat();
			return;
		}

		currentTurn++;
		if (currentTurn >= turnOrder.size())
		{
			currentRound++;
			currentTurn = 0;
		}

		OnStartTurn();
	}

	bool CombatSystem::ShouldFinishCombat()
	{
		Coordinator* coordinator = Coordinator::GetInstance();

		bool allCharactersDead = true;
		for (EntityId entityId : characters)
		{
			CombatComponent& combatComponent = coordinator->GetComponent<CombatComponent>(entityId);
			if (combatComponent.IsAlive())
			{
				allCharactersDead = false;
				break;
			}
		}

		bool allOpponentsDead = true;
		for (EntityId entityId : opponents)
		{
			CombatComponent& combatComponent = coordinator->GetComponent<CombatComponent>(entityId);
			if (combatComponent.IsAlive())
			{
				allOpponentsDead = false;
				break;
			}
		}

		return allCharactersDead or allOpponentsDead;
	}

	void CombatSystem::FinishCombat()
	{
		//todo: call event that combat is finished
		//todo: award xp to characters if won
		turnOrder.clear();
		characters.clear();
		opponents.clear();
		currentTurn = 0;
		currentRound = 1;
		combatOngoing = false;
		OT_INFO("Combat Finished.");
	}

	void CombatSystem::DebugPrintState()
	{
		Coordinator* coordinator = Coordinator::GetInstance();
		OT_INFO("Round [" + std::to_string(currentRound) + "]:");

		for (int i = 0; i < turnOrder.size(); i++)
		{
			EntityId entityId = turnOrder[i];
			std::string toPrint;

			if (i == currentTurn)
				toPrint += "> ";
			else
				toPrint += "- ";

			if (coordinator->HasComponent<CharacterComponent>(entityId))
			{
				CharacterComponent& character = coordinator->GetComponent<CharacterComponent>(entityId);
				CombatComponent& combatComponent = coordinator->GetComponent<CombatComponent>(entityId);
				toPrint += "Lv. " + std::to_string(character.level) + " " + character.name;

				EntityId activePersonaEntityId = character.GetActivePersona();
				PersonaComponent& persona = coordinator->GetComponent<PersonaComponent>(activePersonaEntityId);
				toPrint += " (Lv. " + std::to_string(persona.level) + " " + persona.name + ")";
				toPrint += " - " + std::to_string(combatComponent.hp) + "HP / " + std::to_string(combatComponent.sp) + "SP";
			}
			else if (coordinator->HasComponent<PersonaComponent>(entityId))
			{
				PersonaComponent& opponent = coordinator->GetComponent<PersonaComponent>(entityId);
				CombatComponent& combatComponent = coordinator->GetComponent<CombatComponent>(entityId);

				toPrint += "Lv. " + std::to_string(opponent.level) + " " + opponent.name;
				toPrint += " - " + std::to_string(combatComponent.hp) + "HP / " + std::to_string(combatComponent.sp) + "SP";
			}

			OT_INFO(toPrint);
		}

	}

	bool CombatSystem::IsCharacter(EntityId entityId)
	{
		Coordinator* coordinator = Coordinator::GetInstance();
		return coordinator->HasComponent<CharacterComponent>(entityId);
	}

	std::string CombatSystem::GetName(EntityId entityId)
	{
		Coordinator* coordinator = Coordinator::GetInstance();

		if (IsCharacter(entityId))
		{
			CharacterComponent& character = coordinator->GetComponent<CharacterComponent>(entityId);
			return character.name;
		}
		else
		{
			PersonaComponent& persona = coordinator->GetComponent<PersonaComponent>(entityId);
			return persona.name;
		}
	}

	Level CombatSystem::GetLevel(EntityId entityId)
	{
		Coordinator* coordinator = Coordinator::GetInstance();

		if (IsCharacter(entityId))
		{
			CharacterComponent& character = coordinator->GetComponent<CharacterComponent>(entityId);
			return character.level;
		}
		else
		{
			PersonaComponent& persona = coordinator->GetComponent<PersonaComponent>(entityId);
			return persona.level;
		}
	}
}
