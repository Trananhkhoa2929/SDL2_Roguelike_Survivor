#pragma once

class EntityManager;

class PlayerInputSystem
{
public:
	PlayerInputSystem(float speed = 300.0f) : m_Speed(speed) {}
	void Update(EntityManager& entityManager, float deltaTime);

private:
	float m_Speed;
};
