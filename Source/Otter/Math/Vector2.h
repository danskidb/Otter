#pragma once

namespace Otter {

	class Vector2
	{
	public:
		Vector2() : x(0.0f), y(0.0f) 
		{}

		Vector2(float x, float y) : x(x), y(y)
		{}

		Vector2 operator+(Vector2 const& v)
		{
			return Vector2(x + v.x, y + v.y);
		}

		Vector2 operator+=(Vector2 const& v)
		{
			x += v.x;
			y += v.y;

			return *this;
		}

		Vector2 operator-(Vector2 const& v)
		{
			return Vector2(x - v.x, y - v.y);
		}

		Vector2 operator-=(Vector2 const& v)
		{
			x -= v.x;
			y -= v.y;

			return *this;
		}

		Vector2 operator*(Vector2 const& v)
		{
			return Vector2(x * v.x, y * v.y);
		}

		Vector2 operator*=(Vector2 const& v)
		{
			x *= v.x;
			y *= v.y;

			return *this;
		}

		float x, y;
	};
}