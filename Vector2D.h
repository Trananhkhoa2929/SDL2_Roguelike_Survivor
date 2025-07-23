#pragma once
#include <iostream>
#include <cmath>

class Vector2D
{
public:
	float x;
	float y;
	Vector2D(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
	
	Vector2D operator + (const Vector2D& v2) const { return Vector2D(x + v2.x, y + v2.y); }
	Vector2D operator - (const Vector2D& v2) const { return Vector2D(x - v2.x, y - v2.y); }
	Vector2D operator * (float scalar) const { return Vector2D(x * scalar, y * scalar); }
	
	float length() const {
		return sqrt(x * x + y * y);
	}

	void normalize() {
		float len = length();
		if (len > 0) {
			x /= len;
			y /= len;
		}
	}

	float distanceSq(const Vector2D& v2) const {
		float dx = x - v2.x;
		float dy = y - v2.y;
		return dx * dx + dy * dy;
	}

	friend std::ostream& operator << (std::ostream& out, const Vector2D vec) {
		out << "(" << vec.x << ", " << vec.y << ")";
		return out;
	}
};