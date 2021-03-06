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

		void DestroyEntity(EntityId entityId);

		void SetSignature(EntityId entityId, Signature signature);

		Signature GetSignature(EntityId entityId);

	private:
		// Queue of unused entityIds
		std::queue<EntityId> availableEntities{};

		// Array of signatures where the index corresponds to the entityId
		std::array<Signature, MAX_ENTITIES> signatures{};

		// Total living Entities - used to keep limits on how many exist.
		uint32_t livingEntityCount{};
	};
}
