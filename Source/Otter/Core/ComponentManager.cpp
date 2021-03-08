#include "ComponentManager.h"

namespace Otter {

	void ComponentManager::EntityDestroyed(const EntityId& entityId)
	{
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		for (auto const& pair : componentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entityId);
		}
	}

	void ComponentManager::OnTick(const float& deltaTime)
	{
		for (std::pair<const char*, std::shared_ptr<IComponentArray>> entry : componentArrays)
		{
			std::shared_ptr<IComponentArray> componentArray = entry.second;
			componentArray->OnTick(deltaTime);
		}
	}
}