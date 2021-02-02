#pragma once

#include "Otter/Core.h"
#include "Otter/Math/Vector2.h"

namespace Otter {

	class Transform2DComponent
	{
	public:
		Vector2 translation;
		Vector2 scale;

		Transform2DComponent() {}
		Transform2DComponent(Vector2 _translation, Vector2 _scale) : translation(_translation), scale(_scale) {}
	};
}