#pragma once

#include "Otter/Types.h"
#include <queue>
#include <array>


namespace Otter {

	class EntityManager
	{
	public:
		EntityManager();

		EntityId CreateEntity();

		void DestroyEntity(const EntityId& entityId);

		void SetSignature(const EntityId& entityId, const Signature& signature);

		Signature GetSignature(const EntityId& entityId);

	private:
		// Queue of unused entityIds
		std::queue<EntityId> availableEntities{};

		// Array of signatures where the index corresponds to the entityId
		std::array<Signature, MAX_ENTITIES> signatures{};

		// Total living Entities - used to keep limits on how many exist.
		uint32_t livingEntityCount{};
	};
}
