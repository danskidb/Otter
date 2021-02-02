#include "SandboxApp.h"
#include "Components/PersonaComponent.h"
#include "Components/CharacterComponent.h"
#include "Components/CombatComponent.h"
#include "Systems/CombatSystem.h"
#include "Data/PersonaData.h"

using namespace Otter;

namespace RpgGame {

	void SandboxApp::RegisterComponents()
	{
		Application::RegisterComponents();
		coordinator->RegisterComponent<PersonaComponent>();
		coordinator->RegisterComponent<CharacterComponent>();
		coordinator->RegisterComponent<CombatComponent>();
	}

	void SandboxApp::RegisterSystems()
	{
		Application::RegisterSystems();
		auto combatSystem = coordinator->RegisterSystem<CombatSystem>();
		{
			Signature signature;
			signature.set(coordinator->GetComponentType<CombatComponent>());
			coordinator->SetSystemSignature<CombatSystem>(signature);
		}
	}

	void SandboxApp::OnStart()
	{
		OT_INFO("OnStart");

		// create test character
		EntityId characterEntityId = coordinator->CreateEntity();
		coordinator->AddComponent<CharacterComponent>(characterEntityId, CharacterComponent("Joker", 1, EArcana::Fool));
		coordinator->AddComponent<CombatComponent>(characterEntityId, CombatComponent());

		// create test persona which belongs to the player.
		EntityId personaEntityId = coordinator->CreateEntity();
		coordinator->AddComponent<PersonaComponent>(personaEntityId, PersonaComponent(1, "Arsène", 1, EArcana::Fool));
		coordinator->GetComponent<CharacterComponent>(characterEntityId).instantiatedPersona.push_back(personaEntityId);

		// Create test opponent persona
		EntityId opponentEntityId = coordinator->CreateEntity();
		coordinator->AddComponent<PersonaComponent>(opponentEntityId, PersonaComponent(11, "Jack Frost", 1, EArcana::Magician));
		coordinator->AddComponent<CombatComponent>(opponentEntityId, CombatComponent());

		std::shared_ptr<CombatSystem> combatSystem = coordinator->GetSystem<CombatSystem>();
		combatSystem->InitCombat(std::vector<EntityId>{characterEntityId}, std::vector<EntityId>{opponentEntityId}, ECombatStartType::Normal);

		int bp = 0;
	}

	void SandboxApp::OnTick(float deltaTime)
	{
	}

	void SandboxApp::OnStop()
	{
	}
}