#pragma once

#include "Otter/Core.h"
#include "Otter/Math/Vector2.h"
#include "Otter/Components/Component.h"

namespace Otter {

	class Transform2DComponent : public Component
	{
	public:
		Vector2 translation;
		Vector2 scale;

		Transform2DComponent() : Component() {}
		Transform2DComponent(Vector2 _translation, Vector2 _scale) : Component(), translation(_translation), scale(_scale) {}
	};
}