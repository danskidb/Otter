#pragma once

#include "Otter/Types.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include <memory>

namespace Otter {

	class Coordinator
	{
	public:
		static Coordinator* GetInstance() {
			if (!instance)
			{
				instance = new Coordinator;
				instance->Init();
			}

			return instance;
		}

		static void DestroyInstance() {
			delete(instance);
		}

		void Init();
		
		EntityId CreateEntity();
		
		void DestroyEntity(const EntityId& entityId);

		void OnTick(const float& deltaTime);

		template<typename T> 
		void RegisterComponent()
		{
			componentManager->RegisterComponent<T>();
		}

		template<typename T>
		void AddComponent(const EntityId& entityId, T component)
		{
			componentManager->AddComponent<T>(entityId, component);

			auto signature = entityManager->GetSignature(entityId);
			signature.set(componentManager->GetComponentType<T>(), true);
			entityManager->SetSignature(entityId, signature);

			systemManager->EnitySignatureChanged(entityId, signature);
		}

		template<typename T>
		void RemoveComponent(const EntityId& entityId)
		{
			componentManager->RemoveComponent<T>(entityId);

			auto signature = entityManager->GetSignature(entityId);
			signature.set(componentManager->GetComponentType<T>(), false);
			entityManager->SetSignature(entityId, signature);

			systemManager->EnitySignatureChanged(entityId, signature);
		}

		template<typename T>
		bool HasComponent(const EntityId& entityId)
		{
			Signature s;
			s.set(componentManager->GetComponentType<T>(), true);
			return (entityManager->GetSignature(entityId) & s) == s;
		}

		template<typename T>
		T* GetComponent(const EntityId& entityId)
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
		void SetSystemSignature(const Signature& signature)
		{
			systemManager->SetSignature<T>(signature);
		}

		template<typename T>
		std::shared_ptr<T> GetSystem()
		{
			return systemManager->GetSystem<T>();
		}

	private:
		static Coordinator* instance;
		Coordinator() {};
		Coordinator& operator=(const Coordinator&) = delete;
		Coordinator(const Coordinator&) = delete;

		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<SystemManager> systemManager;
	};
}