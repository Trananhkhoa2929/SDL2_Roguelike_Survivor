#pragma once
#include "IComponent.h"
#include "Vector2D.h"

class Entity;

class TransformComponent : public IComponent
{
public:
	Vector2D position;
	Vector2D velocity;

	int width = 0;
	int height = 0;
	double scale = 1.0;
	double rotation = 0.0;

	// SỬA: Thêm con trỏ để một thực thể có thể đi theo thực thể khác.
	// Dùng cho hào quang của Tỏi đi theo người chơi.
	Entity* entityToFollow = nullptr;
	
	TransformComponent(float x, float y, int w, int h, double s, double r = 0.0)
		: position(x, y), width(w), height(h), scale(s), rotation(r) {}

	void Update(float deltaTime) override {
		position = position + velocity * deltaTime;
	}
};