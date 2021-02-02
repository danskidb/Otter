#pragma once

#include "Types.h"
#include "System.h"
#include <memory>
#include <unordered_map>

namespace Otter {
	
	class SystemManager
	{
	public:
		template<typename T> 
		std::shared_ptr<T> RegisterSystem() 
		{
			const char* typeName = typeid(T).name();

			OT_ASSERT(systems.find(typeName) == systems.end(), "Registering system more than once.");

			// Create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>();
			systems.insert({ typeName, system });
			return system;
		}

		// Register what component types a system requires on an entity
		template<typename T> 
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();

			OT_ASSERT(systems.find(typeName) != systems.end(), "System used before registered.");

			// Set the signature for this system
			signatures.insert({ typeName, signature });
		}

		void EntityDestroyed(EntityId entityId);

		void EnitySignatureChanged(EntityId entityId, Signature entitySignature);

		template<typename T>
		std::shared_ptr<T> GetSystem()
		{
			const char* typeName = typeid(T).name();

			auto itr = systems.find(typeName);
			OT_ASSERT(itr != systems.end(), "Attempted to get system before registered.");

			return std::static_pointer_cast<T>(itr->second);
		}

	private:
		// Map from system type string pointer to a signature
		std::unordered_map<const char*, Signature> signatures{};

		// Map from system type string pointer to a system pointer
		std::unordered_map<const char*, std::shared_ptr<System>> systems{};
	};
}