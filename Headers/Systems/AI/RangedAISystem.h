// file: RangedAISystem.h
#pragma once

class EntityManager;

class RangedAISystem {
public:
    // Tốc độ di chuyển khi kẻ thù cần lại gần người chơi.
    RangedAISystem(float speed = 150.f) : m_RepositionSpeed(speed) {}
    void Update(EntityManager& entityManager, float deltaTime);

private:
    float m_RepositionSpeed;
};