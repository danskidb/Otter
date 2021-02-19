#pragma once

#include "Types.h"
#include "Otter/Log.h"
#include "ComponentArray.h"
#include <unordered_map>
#include <memory>
#include <any>
#include "Otter/Components/Component.h"
#include <type_traits>

namespace Otter {

	class ComponentManager
	{
	public:
		void EntityDestroyed(EntityId entityId);

		void OnTick(float deltaTime);

		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			OT_ASSERT(componentTypes.find(typeName) == componentTypes.end(), "Registering component type more than once.");
			bool based = std::is_base_of<Component, T>::value;
			OT_ASSERT(based, "Attempted to register a component not inheriting from Otter::Component");

			// Add this component type to the component type map
			componentTypes.insert({ typeName, mNextComponentType });

			// Create a ComponentArray pointer and add it to the component arrays map
			componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

			// Increment the value so that the next component registered will be different
			++mNextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			OT_ASSERT(componentTypes.find(typeName) != componentTypes.end(), "Component not registered before use.");

			// Return this component's type - used for creating signatures
			return componentTypes[typeName];
		}

		template<typename T>
		void AddComponent(EntityId entityId, T component)
		{
			// Add a component to the array for an entity
			GetComponentArray<T>()->InsertData(entityId, component);
		}

		template<typename T>
		void RemoveComponent(EntityId entityId)
		{
			// Remove a component from the array for an entity
			GetComponentArray<T>()->RemoveData(entityId);
		}

		template<typename T>
		T& GetComponent(EntityId entityId)
		{
			// Get a reference to a component from the array for an entity
			return GetComponentArray<T>()->GetData(entityId);
		}

	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> componentTypes{};

		// Map from type string pointer to a component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType mNextComponentType{};

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			OT_ASSERT(componentTypes.find(typeName) != componentTypes.end(), "Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
		}
	};

}


