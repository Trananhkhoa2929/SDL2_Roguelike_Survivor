#pragma once
#include "IComponent.h"

class HealthComponent : public IComponent
{
public:
	int currentHealth;
	int maxHealth;

	HealthComponent(int health) : currentHealth(health), maxHealth(health) {}
};

