#include "CombatSystem.h"
#include "Otter/Utils/Log.h"
#include "Otter/Core/Coordinator.h"

#include "Components/CharacterComponent.h"
#include "Components/PersonaComponent.h"
#include "Components/CombatComponent.h"

#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include "Compendium.h"

namespace RpgGame {

	void CombatSystem::InitCombat(std::vector<EntityId> _characters, std::vector<EntityId> _opponents, ECombatStartType _combatStartType)
	{
		Coordinator* coordinator = Coordinator::GetInstance();

		OT_ASSERT(!combatOngoing, "Called to initialize combat while it was already ongoing!");
		combatOngoing = true;

		// Verify characters
		OT_ASSERT(_characters.size() <= maxCharacters, "Attempted to start combat with " + std::to_string(_characters.size()) + " characters, but the max supported is " + std::to_string(maxCharacters));
		for (EntityId characterEntity : _characters)
		{
			OT_ASSERT(coordinator->HasComponent<CharacterComponent>(characterEntity), "Character does not have a CharacterComponent!");
			CharacterComponent* character = coordinator->GetComponent<CharacterComponent>(characterEntity);
			Equipment* e = &character->allEquipment[EEquipmentSlot::RangeWeapon];
			e->RefillAmmo();
		}
		characters = _characters;

		// Verify opponents
		OT_ASSERT(_opponents.size() <= maxOpponents, "Attempted to start combat with " + std::to_string(_opponents.size()) + " opponents, but the max supported is " + std::to_string(maxOpponents));
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

		unsigned int seed = (unsigned int) std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(tempVector.begin(), tempVector.end(), std::default_random_engine((unsigned int) seed));

		turnOrder = tempVector;
	}

	void CombatSystem::OnStartTurn(bool isRestart)
	{
		Coordinator* coordinator = Coordinator::GetInstance();
		EntityId entity = turnOrder[currentTurn];
		CombatComponent* combatData = coordinator->GetComponent<CombatComponent>(entity);

		if (currentTurn == 0)
			DebugPrintState();

		if (!combatData->IsAlive())
		{
			EndTurn();
			return;
		}

		// If there's a character, it means that we can control it.
		if (IsCharacter(entity)) 
		{
			ECombatAction playerAction = AskPlayerAction();
			CharacterComponent* character = coordinator->GetComponent<CharacterComponent>(entity);

			switch (playerAction)
			{
				case ECombatAction::Melee: {
					EntityId target = AskPlayerTarget();
					PerformMeleeAttack(entity, target);
					break;
				}

				case ECombatAction::Gun: {
					Equipment* e = &character->allEquipment[EEquipmentSlot::RangeWeapon];
					if (e->rounds > 0)
					{
						int shotsToFire = e->rounds;
						for (int i = 0; i < shotsToFire; i++)
						{
							EntityId target = AskPlayerTarget();
							PerformRangedAttack(entity, target);
						}
					}
					else
					{
						OT_WARN("" + GetName(entity) + " has no bullets remaining!");
						OnStartTurn(true);
					}
					break;
				}

				case ECombatAction::Skill: {
					CombatSkill skill;
					bool result = AskPlayerSkill(skill);
					if (!result)
					{
						OnStartTurn(true);
					}
					else
					{
						// todo targetRestrictions
						EntityId target = AskPlayerTarget();
						PerformSkill(entity, target, skill);
					}
				}

				default: {
					OT_ERROR("An invalid action was chosen!");
				}
			}
		}
		else
		{
			//todo: parse AI behaviour
			EntityId target = GetRandomEnemyTarget(entity);
			PerformMeleeAttack(entity, target);
		}
	}

	EntityId CombatSystem::GetRandomEnemyTarget(EntityId toPickFor)
	{
		unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);

		if (IsCharacter(toPickFor))
		{
			std::vector<EntityId> aliveOpponents = FilterAliveEntities(opponents);
			std::uniform_int_distribution<size_t> distribution(0, aliveOpponents.size() - 1);
			return aliveOpponents[distribution(generator)];
		}
		else
		{
			std::vector<EntityId> aliveCharacters = FilterAliveEntities(characters);
			std::uniform_int_distribution<size_t> distribution(0, aliveCharacters.size() - 1);
			return aliveCharacters[distribution(generator)];
		}
	}

	std::vector<EntityId> CombatSystem::FilterAliveEntities(std::vector<EntityId> characterOrOpponentVector)
	{
		Coordinator* coordinator = Coordinator::GetInstance();
		std::vector<EntityId> result;

		for (EntityId entity : characterOrOpponentVector)
		{
			CombatComponent* combatData = coordinator->GetComponent<CombatComponent>(entity);
			if (combatData->IsAlive())
				result.push_back(entity);
		}

		return result;
	}

	float CombatSystem::GetCharacterAttackPower(Equipment* meleeOrRangedWeapon, CombatStat* attackerStats)
	{
		OT_ASSERT(meleeOrRangedWeapon != nullptr, "GetCharacterAttackPower: meleeOrRangedWeapon was a nullptr!");
		OT_ASSERT(attackerStats != nullptr, "GetCharacterAttackPower: attackerStats was a nullptr!");
		OT_ASSERT(meleeOrRangedWeapon->slot == EEquipmentSlot::MeleeWeapon || meleeOrRangedWeapon->slot == EEquipmentSlot::RangeWeapon, "GetCharacterAttackPower: Attempted to get Melee Attack Damage with a non-melee weapon! Equipment used: " + meleeOrRangedWeapon->name);
		return std::sqrtf((float)meleeOrRangedWeapon->attack) * std::sqrtf((float)attackerStats->strength);
	}

	float CombatSystem::SubtractDefense(float baseAttackDamage, CombatStat* defendantStats, Equipment* defendantArmor)
	{
		OT_ASSERT(defendantStats != nullptr, "SubtractDefense: defendantStats was a nullptr.");
		float result = baseAttackDamage;

		unsigned int armor = 0;
		if (defendantArmor != nullptr)
		{
			OT_ASSERT(defendantArmor->slot == EEquipmentSlot::Armor, "SubtractDefense: Attempted to get Armor defense with non-armor! Equipment used: " + defendantArmor->name);
			armor = defendantArmor->defense;
		}
	
		result /= std::sqrtf((float)((defendantStats->endurance * 8) + armor));
		return result;
	}

	void CombatSystem::PerformMeleeAttack(EntityId performer, EntityId target)
	{
		Coordinator* coordinator = Coordinator::GetInstance();
		CombatComponent* targetCombatComponent = coordinator->GetComponent<CombatComponent>(target);

		int dmg;
		if (coordinator->HasComponent<CharacterComponent>(performer))
		{
			CharacterComponent* character = coordinator->GetComponent<CharacterComponent>(performer);
			PersonaComponent* persona = coordinator->GetComponent<PersonaComponent>(performer);
			PersonaComponent* defendant = coordinator->GetComponent<PersonaComponent>(target);

			Equipment* e = &character->allEquipment[EEquipmentSlot::MeleeWeapon];
			float atkDmg = GetCharacterAttackPower(e, &persona->stats);
			dmg = std::lrint(SubtractDefense(atkDmg, &defendant->stats, nullptr));
		}
		else
		{
			//temporary
			dmg = GetLevel(performer);
		}

		targetCombatComponent->hp -= dmg;
		OT_INFO(GetName(performer) + " attacked " + GetName(target) + " with Melee for " + std::to_string(dmg) + " damage.");

		if (!targetCombatComponent->IsAlive())
			OT_INFO(GetName(target) + " fainted.");

		EndTurn();
	}

	void CombatSystem::PerformRangedAttack(EntityId performer, EntityId target)
	{
		Coordinator* coordinator = Coordinator::GetInstance();

		OT_ASSERT(coordinator->HasComponent<CharacterComponent>(performer), "PerformRangedAttack: performer is not a character!");
		CharacterComponent* character = coordinator->GetComponent<CharacterComponent>(performer);

		PersonaComponent* persona = coordinator->GetComponent<PersonaComponent>(performer);
		PersonaComponent* defendant = coordinator->GetComponent<PersonaComponent>(target);
		CombatComponent* defendantCombatComponent = coordinator->GetComponent<CombatComponent>(target);

		Equipment* e = &character->allEquipment[EEquipmentSlot::RangeWeapon];
		if (e->rounds <= 0)
		{
			OT_INFO(GetName(performer) + " attempted to fire a gun with no rounds remaining.");
			return;
		}

		float atkDmg = GetCharacterAttackPower(e, &persona->stats);
		int dmg = std::lround(SubtractDefense(atkDmg, &defendant->stats, nullptr));
		e->rounds--;

		defendantCombatComponent->hp -= dmg;
		OT_INFO(GetName(performer) + " attacked " + GetName(target) + " with Range for " + std::to_string(dmg) + " damage. They have " + std::to_string(e->rounds) + " rounds left");

		if (!defendantCombatComponent->IsAlive())
			OT_INFO(GetName(target) + " fainted.");

		// todo: if fired a gun this round, you can't do any action other than performing a ranged attack. You can pick a different target or stop to perserve ammo.
		if (e->rounds <= 0)
			EndTurn();
	}

	void CombatSystem::PerformSkill(EntityId performer, EntityId target, CombatSkill skill)
	{
		Coordinator* coordinator = Coordinator::GetInstance();
		PersonaComponent* performerPersona = coordinator->GetComponent<PersonaComponent>(performer);
		CombatComponent* performerCombatData = coordinator->GetComponent<CombatComponent>(performer);

		PersonaComponent* targetPersona = coordinator->GetComponent<PersonaComponent>(target);
		CombatComponent* targetCombatData = coordinator->GetComponent<CombatComponent>(target);

		// subtract cost
		switch (skill.costType)
		{
			case ECombatSkillCostType::Fixed_SP:
				performerCombatData->sp -= skill.cost;
				break;
			case ECombatSkillCostType::Percentage_HP:
				performerCombatData->hp -= (skill.cost / 100) * performerCombatData->hp;
				break;
		}

		// Decide effectiveness based on target Affinity. (store in multiplier)
		// Calculate how often this should attack (chance calculation between min-max, if max > min)
		// For each times should attack
			// loop through all effects
			// get random ailment to inflict from list & check if we apply it.
			
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
			CombatComponent* combatComponent = coordinator->GetComponent<CombatComponent>(entityId);
			if (combatComponent->IsAlive())
			{
				allCharactersDead = false;
				break;
			}
		}

		bool allOpponentsDead = true;
		for (EntityId entityId : opponents)
		{
			CombatComponent* combatComponent = coordinator->GetComponent<CombatComponent>(entityId);
			if (combatComponent->IsAlive())
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

	bool CombatSystem::IsPlayerInputValid(std::string userInput, uint8_t amountOptions)
	{
		if (userInput.size() != 1)
		{
			OT_WARN("Invalid input length. Please try again");
			return false;
		}

		bool isNumber = !userInput.empty() && std::find_if(userInput.begin(), userInput.end(), [](unsigned char c) { return !std::isdigit(c); }) == userInput.end();
		if (!isNumber)
		{
			OT_WARN("Invalid input, NaN. Please try again");
			return false;
		}

		int entry = std::stoi(userInput);
		if (entry < 0 || entry >= (int)amountOptions)//entry >= (int)ECombatAction::_MAX_ENTRY)
		{
			OT_WARN("Invalid input, outside of range. Please try again");
			return false;
		}

		return true;
	}

	//////////////////////////////////////////////////////
	// Console & Player interaction
	ECombatAction CombatSystem::AskPlayerAction()
	{
		OT_INFO("Select action: [0] Melee, [1] Gun, [2] Skill");
		std::string userInput;
		std::cin >> userInput;

		if (!IsPlayerInputValid(userInput, 3))
			return AskPlayerAction();

		int entry = std::stoi(userInput);
		return static_cast<ECombatAction>(entry);
	}

	bool CombatSystem::AskPlayerSkill(CombatSkill &outSkill)
	{
		Coordinator* coordinator = Coordinator::GetInstance();
		std::shared_ptr<Compendium> compendium = coordinator->GetSystem<Compendium>();

		EntityId currentTurnEntity = turnOrder[currentTurn];
		OT_ASSERT(IsCharacter(currentTurnEntity), "Attempted to ask for a skill on a non-character!");

		PersonaComponent* persona = coordinator->GetComponent<PersonaComponent>(currentTurnEntity);
		std::string userChoice = "Select Skill: ";

		std::vector<CombatSkill> skills = compendium->FindSkillsById(persona->activeSkills);
		if (skills.size() <= 0)
			return false;	//player has no skills so we can't ask them for it.

		for (int i = 0; i < skills.size(); ++i)
		{
			userChoice += "[" + std::to_string(i) + "] " + skills[i].name + " ";
		}
		userChoice += "[" + std::to_string(skills.size()) + "] Return";

		OT_INFO(userChoice);
		std::string userInput;
		std::cin >> userInput;

		if (!IsPlayerInputValid(userInput, (uint8_t)skills.size() + 1))
			return AskPlayerTarget();

		int entry = std::stoi(userInput);
		if (entry == skills.size())
		{
			return false;	// user wants to return
		}

		outSkill = skills[entry];
		return true;
	}

	EntityId CombatSystem::AskPlayerTarget()
	{
		EntityId currentTurnEntity = turnOrder[currentTurn];
		OT_ASSERT(IsCharacter(currentTurnEntity), "Attempted to ask for a player target on a non-character!");

		std::string userChoice = "Select Target: ";
		std::vector<EntityId> aliveEntities = FilterAliveEntities(turnOrder);
		for (int i = 0; i < aliveEntities.size(); ++i)
		{
			userChoice += "[" + std::to_string(i) + "] " + GetName(aliveEntities[i]) + " ";
		}

		OT_INFO(userChoice);
		std::string userInput;
		std::cin >> userInput;

		if (!IsPlayerInputValid(userInput, (int)aliveEntities.size()))
			return AskPlayerTarget();

		int entry = std::stoi(userInput);
		return aliveEntities[entry];
	}

	void CombatSystem::DebugPrintState()
	{
		Coordinator* coordinator = Coordinator::GetInstance();
		OT_INFO("Round [" + std::to_string(currentRound) + "]:");

		for (int i = 0; i < turnOrder.size(); i++)
		{
			EntityId entityId = turnOrder[i];

			CombatComponent* combatComponent = coordinator->GetComponent<CombatComponent>(entityId);
			if (!combatComponent->IsAlive())
				continue;

			std::string toPrint;

			if (i == currentTurn)
				toPrint += "> ";
			else
				toPrint += "- ";

			PersonaComponent* persona = coordinator->GetComponent<PersonaComponent>(entityId);
			if (coordinator->HasComponent<CharacterComponent>(entityId))
			{
				CharacterComponent* character = coordinator->GetComponent<CharacterComponent>(entityId);

				toPrint += "Lv. " + std::to_string(character->level) + " " + character->codename;
				toPrint += " (Lv. " + std::to_string(persona->level) + " " + persona->name + ")";
				toPrint += " - " + std::to_string(combatComponent->hp) + "HP / " + std::to_string(combatComponent->sp) + "SP";
			}
			else if (coordinator->HasComponent<PersonaComponent>(entityId))
			{
				toPrint += "Lv. " + std::to_string(persona->level) + " " + persona->name;
				toPrint += " - " + std::to_string(combatComponent->hp) + "HP / " + std::to_string(combatComponent->sp) + "SP";
			}

			OT_INFO(toPrint);
		}

	}


	//////////////////////////////////////////////////////
	// Helper Functions
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
			CharacterComponent* character = coordinator->GetComponent<CharacterComponent>(entityId);
			return character->codename;
		}
		else
		{
			PersonaComponent* persona = coordinator->GetComponent<PersonaComponent>(entityId);
			return persona->name;
		}
	}

	Level CombatSystem::GetLevel(EntityId entityId)
	{
		Coordinator* coordinator = Coordinator::GetInstance();

		if (IsCharacter(entityId))
		{
			CharacterComponent* character = coordinator->GetComponent<CharacterComponent>(entityId);
			return character->level;
		}
		else
		{
			PersonaComponent* persona = coordinator->GetComponent<PersonaComponent>(entityId);
			return persona->level;
		}
	}
}
