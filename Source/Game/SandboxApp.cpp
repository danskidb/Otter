#include "SandboxApp.h"
#include "Components/PersonaComponent.h"
#include "Components/CharacterComponent.h"
#include "Components/CombatComponent.h"
#include "Systems/CombatSystem.h"
#include "Data/PersonaData.h"
#include "Otter/Utils/Filesystem.h"
#include "Data/Compendium.h"

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

		// Load compendium from JSON
		Compendium c;
		c.Initialize();

		Persona p;
		bool result = c.FindPersonaById("Arsene", p);
		std::vector<Persona> byArcana = c.FindPersonaByArcana(EArcana::Councillor);

		// create test character with owning persona.
		EntityId characterEntityId = coordinator->CreateEntity();
		coordinator->AddComponent<CharacterComponent>(characterEntityId, CharacterComponent("Joker", 1, EArcana::Fool));
		coordinator->AddComponent<CombatComponent>(characterEntityId, CombatComponent(100, 50));

		EntityId personaEntityId = coordinator->CreateEntity();
		coordinator->AddComponent<PersonaComponent>(personaEntityId, PersonaComponent(1, "Arsène", 1, EArcana::Fool));
		coordinator->GetComponent<CharacterComponent>(characterEntityId).AddPersona(personaEntityId);

		// Create test opponent persona
		EntityId opponent1 = coordinator->CreateEntity();
		coordinator->AddComponent<PersonaComponent>(opponent1, PersonaComponent(2, "Jack Frost", 11, EArcana::Magician));
		coordinator->AddComponent<CombatComponent>(opponent1, CombatComponent(100, 100));

		EntityId opponent2 = coordinator->CreateEntity();
		coordinator->AddComponent<PersonaComponent>(opponent2, PersonaComponent(3, "Pixie", 2, EArcana::Lovers));
		coordinator->AddComponent<CombatComponent>(opponent2, CombatComponent(10, 20));

		EntityId opponent3 = coordinator->CreateEntity();
		coordinator->AddComponent<PersonaComponent>(opponent3, PersonaComponent(3, "Pixie", 2, EArcana::Lovers));
		coordinator->AddComponent<CombatComponent>(opponent3, CombatComponent(10, 20));

		// Run combat
		std::shared_ptr<CombatSystem> combatSystem = coordinator->GetSystem<CombatSystem>();
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