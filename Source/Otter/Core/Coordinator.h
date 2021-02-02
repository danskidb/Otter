#pragma once

#include "Types.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include <memory>

namespace Otter {

	class Coordinator
	{
	public:
		void Init();
		
		EntityId CreateEntity();
		
		void DestroyEntity(EntityId entityId);

		template<typename T> 
		void RegisterComponent()
		{
			componentManager->RegisterComponent<T>();
		}

		template<typename T>
		void AddComponent(EntityId entityId, T component)
		{
			componentManager->AddComponent<T>(entityId, component);

			auto signature = entityManager->GetSignature(entityId);
			signature.set(componentManager->GetComponentType<T>(), true);
			entityManager->SetSignature(entityId, signature);

			systemManager->EnitySignatureChanged(entityId, signature);
		}

		template<typename T>
		void RemoveComponent(EntityId entityId)
		{
			componentManager->RemoveComponent<T>(entityId);

			auto signature = entityManager->GetSignature(entityId);
			signature.set(componentManager->GetComponentType<T>(), false);
			entityManager->SetSignature(entityId, signature);

			systemManager->EnitySignatureChanged(entityId, signature);
		}

		template<typename T>
		bool HasComponent(EntityId entityId)
		{
			Signature s;
			s.set(componentManager->GetComponentType<T>(), true);
			return (entityManager->GetSignature(entityId) & s) == s;
		}

		template<typename T>
		T& GetComponent(EntityId entityId)
		{
			return componentManager->GetComponent<T>(entityId);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return componentManager->GetComponentType<T>();
		}

		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			return systemManager->RegisterSystem<T>();
		}

		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			systemManager->SetSignature<T>(signature);
		}

		template<typename T>
		std::shared_ptr<T> GetSystem()
		{
			return systemManager->GetSystem<T>();
		}

	private:
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<SystemManager> systemManager;
	};
}