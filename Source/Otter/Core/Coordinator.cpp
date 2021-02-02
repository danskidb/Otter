#include "Coordinator.h"

namespace Otter {

	void Coordinator::Init()
	{
		componentManager = std::make_unique<ComponentManager>();
		entityManager = std::make_unique<EntityManager>();
		systemManager = std::make_unique<SystemManager>();
	}

	EntityId Coordinator::CreateEntity()
	{
		return entityManager->CreateEntity();
	}

	void Coordinator::DestroyEntity(EntityId entityId)
	{
		entityManager->DestroyEntity(entityId);
		componentManager->EntityDestroyed(entityId);
		systemManager->EntityDestroyed(entityId);
	}
}