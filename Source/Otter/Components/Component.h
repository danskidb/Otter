#pragma once

namespace Otter {

	class Component {
	protected:
		bool componentWantsTick = false;

	public:
		Component() {};

		bool GetComponentWantsTick() { return componentWantsTick; }

		// Called when the component is created
		virtual void OnComponentAdded() {}

		// Called when the component is removed
		virtual void OnComponentRemoved() {}

		// Called before OnComponentRemoved, when the ComponentManager becomes aware that the entity is destroyed.
		virtual void OnEntityPreDestruct() {}

		// Called every update loop
		virtual void OnTick(float deltaTime) {}
	};
}