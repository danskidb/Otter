#include "SandboxApp.h"
#include "Components/PersonaComponent.h"
#include "Components/CharacterComponent.h"
#include "Components/CombatComponent.h"
#include "Systems/CombatSystem.h"
#include "Systems/Compendium.h"
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

		auto compendium = coordinator->RegisterSystem<Compendium>();
	}

	void SandboxApp::OnStart()
	{
		OT_INFO("OnStart");

		std::shared_ptr<Compendium> compendium = coordinator->GetSystem<Compendium>();
		std::shared_ptr<CombatSystem> combatSystem = coordinator->GetSystem<CombatSystem>();

		// create test character with owning persona.
		EntityId characterEntityId = coordinator->CreateEntity();
		CharacterComponent mcCharacter;
		bool result0 = compendium->FindCharacterById("Joker", mcCharacter);

		PersonaComponent mcPersona;
		bool result1 = compendium->FindPersonaById(mcCharacter.personaId, mcPersona);	//todo: read from save file. Persona will have leveled up.
		coordinator->AddComponent<CharacterComponent>(characterEntityId, mcCharacter);
		coordinator->AddComponent<CombatComponent>(characterEntityId, CombatComponent(100, 50));
		coordinator->AddComponent<PersonaComponent>(characterEntityId, mcPersona);

		// Create test opponent persona
		EntityId opponent1 = coordinator->CreateEntity();
		PersonaComponent opponent1Persona;
		bool result2 = compendium->FindPersonaById("Jack Frost", opponent1Persona);
		coordinator->AddComponent<PersonaComponent>(opponent1, opponent1Persona);
		coordinator->AddComponent<CombatComponent>(opponent1, CombatComponent(100, 100));

		EntityId opponent2 = coordinator->CreateEntity();
		PersonaComponent opponent2Persona;
		bool result3 = compendium->FindPersonaById("Pixie", opponent2Persona);
		coordinator->AddComponent<PersonaComponent>(opponent2, opponent2Persona);
		coordinator->AddComponent<CombatComponent>(opponent2, CombatComponent(10, 20));

		EntityId opponent3 = coordinator->CreateEntity();
		coordinator->AddComponent<PersonaComponent>(opponent3, opponent2Persona);
		coordinator->AddComponent<CombatComponent>(opponent3, CombatComponent(10, 20));

		// Run combat
		combatSystem->InitCombat(std::vector<EntityId>{characterEntityId}, std::vector<EntityId>{opponent1, opponent2, opponent3}, ECombatStartType::Normal);

		int breakpoint = 0;
	}

	void SandboxApp::OnTick(float deltaTime)
	{
	}

	void SandboxApp::OnStop()
	{
	}
}