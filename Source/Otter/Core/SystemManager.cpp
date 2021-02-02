#include "SystemManager.h"

namespace Otter {

	void SystemManager::EntityDestroyed(EntityId entityId)
	{
		// Erase a destroyed entity from all system lists
		// mEntities is a set so no check needed
		for (auto const& pair : systems)
		{
			auto const& system = pair.second;
			system->entities.erase(entityId);
		}
	}

	void SystemManager::EnitySignatureChanged(EntityId entityId, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = signatures[type];

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