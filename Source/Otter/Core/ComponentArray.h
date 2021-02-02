#pragma once

#include "Types.h"
#include <unordered_map>
#include <array>

namespace Otter {

	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;	// make virtual, ensure default deconstructor being fired in ComponentArray when destroying an IComponentArray
		virtual void EntityDestroyed(EntityId entityId) = 0; // pure virtual func, ensure implementation
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void InsertData(EntityId entityId, T component)
		{
			OT_ASSERT(entityToIndexMap.find(entityId) == entityToIndexMap.end(), "Component added to the same entity more than once.");

			// Put the new entry at the end, update lookup tables.
			size_t newIndex = mSize;
			entityToIndexMap[entityId] = newIndex;
			indexToEntityMap[newIndex] = entityId;
			componentArray[newIndex] = component;

			++mSize;
		}

		void RemoveData(EntityId entityId)
		{
			OT_ASSERT(entityToIndexMap.find(entityId) != entityToIndexMap.end(), "Removing non-existent component.");

			// Copy element at end into deleted element's place to maintain density
			size_t indexOfRemovedEntity = entityToIndexMap[entityId];
			size_t indexOfLastElement = mSize - 1;
			componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

			// Update map to point to moved spot
			EntityId entityOfLastElement = indexToEntityMap[indexOfLastElement];
			entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			entityToIndexMap.erase(entityId);
			indexToEntityMap.erase(indexOfLastElement);

			--mSize;
		}

		T& GetData(EntityId entityId)
		{
			OT_ASSERT(entityToIndexMap.find(entityId) != entityToIndexMap.end(), "Retrieving non-existent component.");
			return componentArray[entityToIndexMap[entityId]];
		}

		void EntityDestroyed(EntityId entityId) override
		{
			if (entityToIndexMap.find(entityId) != entityToIndexMap.end())
			{
				// Remove the entity's component if it existed
				RemoveData(entityId);
			}
		}

	private:
		// spots for this component on each entity.
		std::array<T, MAX_ENTITIES> componentArray{};

		std::unordered_map<EntityId, size_t> entityToIndexMap{};

		std::unordered_map<size_t, EntityId> indexToEntityMap{};

		size_t mSize{};
	};
}