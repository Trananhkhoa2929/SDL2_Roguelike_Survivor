#pragma once

class EntityManager;

class AISystem
{
public:
    AISystem(float speed = 200.f) : m_Speed(speed) {}
    void Update(EntityManager& entityManager, float deltaTime);
private:
    float m_Speed;
};