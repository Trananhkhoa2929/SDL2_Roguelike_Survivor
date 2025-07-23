#pragma once
#include "IComponent.h"
#include "Vector2D.h"

class ProjectileComponent : public IComponent
{
public:
	int damage;
	float maxRange;
	Vector2D startPosition;
	ProjectileComponent(int dmg, float range, Vector2D startPos)
		: damage(dmg), maxRange(range), startPosition(startPos) {}
};