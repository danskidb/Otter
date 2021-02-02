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
		std::shared_ptr<T> RegisterSystem();

		// Register what component types a system requires on an entity
		template<typename T>
		void SetSignature(Signature signature);

		void EntityDestroyed(EntityId entityId);

		void EnitySignatureChanged(EntityId entityId, Signature entitySignature);

	private:
		// Map from system type string pointer to a signature
		std::unordered_map<const char*, Signature> signatures{};

		// Map from system type string pointer to a system pointer
		std::unordered_map<const char*, std::shared_ptr<System>> systems{};
	};
}