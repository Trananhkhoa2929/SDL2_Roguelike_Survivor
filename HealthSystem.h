#pragma once

class EntityManager;

class HealthSystem
{
public:
	void Update(EntityManager& entityManager, float deltaTime);
};