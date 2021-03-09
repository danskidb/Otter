#include "SystemManager.h"

namespace Otter {

	void SystemManager::EntityDestroyed(const EntityId& entityId)
	{
		// Erase a destroyed entity from all system lists
		// mEntities is a set so no check needed
		for (const auto& pair : systems)
		{
			const auto& system = pair.second;
			system->entities.erase(entityId);
		}
	}

	void SystemManager::EnitySignatureChanged(const EntityId& entityId, const Signature& entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (const auto& pair : systems)
		{
			const auto& type = pair.first;
			const auto& system = pair.second;
			const auto& systemSignature = signatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				// Entity signature matches system signature - insert into set
				system->entities.insert(entityId);
			}
			else
			{
				// Entity signature does not match system signature - erase from set
				system->entities.erase(entityId);
			}
		}
	}
}