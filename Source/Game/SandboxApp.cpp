#include "SandboxApp.h"
#include "Components/PersonaComponent.h"
#include "Components/CharacterComponent.h"
#include "Data/PersonaData.h"

using namespace Otter;
using namespace RpgGame;

SandboxApp::SandboxApp()
{
}

SandboxApp::~SandboxApp()
{
}

void SandboxApp::RegisterComponents()
{
	Application::RegisterComponents();
	coordinator.RegisterComponent<PersonaComponent>();
	coordinator.RegisterComponent<CharacterComponent>();
}

void SandboxApp::OnStart()
{
	OT_INFO("OnStart");

	// create test character
	EntityId characterEntityId = coordinator.CreateEntity();
	coordinator.AddComponent<CharacterComponent>(characterEntityId, CharacterComponent("Joker", 1, EArcana::Fool));

	// create test persona
	EntityId personaEntityId = coordinator.CreateEntity();
	coordinator.AddComponent<PersonaComponent>(personaEntityId, PersonaComponent(1, "Arsène", 1, EArcana::Fool));
	CharacterComponent& characterComponent = coordinator.GetComponent<CharacterComponent>(characterEntityId);
	characterComponent.instantiatedPersona.push_back(personaEntityId);

	int bp = 0;
}

void SandboxApp::OnTick(float deltaTime)
{
}

void SandboxApp::OnStop()
{
}
