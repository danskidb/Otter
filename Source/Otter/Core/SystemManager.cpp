#include "SystemManager.h"

namespace Otter {

	template<typename T>
	std::shared_ptr<T> SystemManager::RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		OT_ASSERT(systems.find(typeName) == systems.end(), "Registering system more than once.");

		// Create a pointer to the system and return it so it can be used externally
		auto system = std::make_shared<T>();
		systems.insert({ typeName, system });
		return system;
	}

	template<typename T>
	void SystemManager::SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		OT_ASSERT(systems.find(typeName) != systems.end(), "System used before registered.");

		// Set the signature for this system
		signatures.insert({ typeName, signature });
	}

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