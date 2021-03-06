#include "EntityManager.h"
#include "Otter/Utils/Log.h"

namespace Otter {

	EntityManager::EntityManager()
	{
		for (EntityId e = 0; e < MAX_ENTITIES; e++)
		{
			availableEntities.push(e);
		}
	}

	EntityId EntityManager::CreateEntity()
	{
		OT_ASSERT(livingEntityCount < MAX_ENTITIES, "Too Many Entities in Existence");

		// Take an ID from the front of the queue
		EntityId id = availableEntities.front();
		availableEntities.pop();
		++livingEntityCount;

		return id;
	}

	void EntityManager::DestroyEntity(EntityId entityId)
	{
		OT_ASSERT(entityId < MAX_ENTITIES, "Entity out of range.");

		// Invalidate the signature & return the destroyed id to the back of the queue.
		signatures[entityId].reset();
		availableEntities.push(entityId);
		--livingEntityCount;
	}

	void EntityManager::SetSignature(EntityId entityId, Signature signature)
	{
		OT_ASSERT(entityId < MAX_ENTITIES, "Entity out of range.");
		signatures[entityId] = signature;
	}

	Signature EntityManager::GetSignature(EntityId entityId)
	{
		OT_ASSERT(entityId < MAX_ENTITIES, "Entity out of range.");
		return signatures[entityId];
	}

}