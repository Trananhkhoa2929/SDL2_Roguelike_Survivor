#include "AISystem.h"
#include "AIComponent.h"
#include "TransformComponent.h"
#include "EntityManager.h"
#include "PlayerControllerComponent.h"
#include "Vector2D.h"

void AISystem::Update(EntityManager& entityManager, float deltaTime) {
    Vector2D playerPosition;
    bool playerFound = false;

    // 1. Tìm vị trí của người chơi
    for (auto& entity : entityManager.GetEntities()) {
        if (entity->HasComponent<PlayerControllerComponent>()) {
            playerPosition = entity->GetComponent<TransformComponent>().position;
            playerFound = true;
            break;
        }
    }

    if (!playerFound) return; // Nếu không có người chơi, không làm gì cả

    // 2. Cập nhật tất cả các AI
    for (auto& entity : entityManager.GetEntities()) {
        if (entity->HasComponent<AIComponent>()) {
            auto& transform = entity->GetComponent<TransformComponent>();
            Vector2D direction = playerPosition - transform.position;
            direction.normalize();
            transform.velocity = direction * m_Speed;
        }
    }
}