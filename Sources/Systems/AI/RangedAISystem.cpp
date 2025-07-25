// file: RangedAISystem.cpp
#include "RangedAISystem.h"
#include "EntityManager.h"
#include "RangedAIComponent.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "ProjectileComponent.h"
#include "Vector2D.h"

void RangedAISystem::Update(EntityManager& entityManager, float deltaTime) {
    Vector2D playerPosition;
    bool playerFound = false;

    // 1. Tìm vị trí người chơi
    for (auto& entity : entityManager.GetEntities()) {
        if (entity->HasComponent<PlayerControllerComponent>()) {
            playerPosition = entity->GetComponent<TransformComponent>().position;
            playerFound = true;
            break;
        }
    }
    if (!playerFound) return;

    // 2. Lặp qua các entity có RangedAIComponent
    for (auto& archer : entityManager.GetEntities()) {
        if (!archer->IsActive() || !archer->HasComponent<RangedAIComponent>()) continue;

        auto& archerTransform = archer->GetComponent<TransformComponent>();
        auto& archerAI = archer->GetComponent<RangedAIComponent>();

        float distanceToPlayer = archerTransform.position.distanceSq(playerPosition);

        // 3. Quyết định hành vi dựa trên khoảng cách
        if (distanceToPlayer <= (archerAI.attackRange * archerAI.attackRange)) {
            // ----- HÀNH VI TẤN CÔNG (TRONG TẦM BẮN) -----
            archerTransform.velocity = { 0, 0 }; // Đứng yên để bắn

            archerAI.timer += deltaTime;
            if (archerAI.timer >= archerAI.attackCooldown) {
                archerAI.timer = 0.0f; // Reset cooldown

                // Bắn tên!
                Vector2D direction = playerPosition - archerTransform.position;
                direction.normalize();

                Entity& arrow = entityManager.AddEntity();
                arrow.AddComponent<TransformComponent>(archerTransform.position.x, archerTransform.position.y, 16, 16, 1.5);
                arrow.GetComponent<TransformComponent>().velocity = direction * 400.0f; // Tốc độ của mũi tên
                arrow.AddComponent<SpriteComponent>("arrow_sprite"); // Cần texture mới
                arrow.AddComponent<ColliderComponent>("enemy_projectile"); // Tag mới!
                arrow.AddComponent<ProjectileComponent>(10, archerAI.attackRange + 50.f, archerTransform.position);
            }
        }
        else {
            // ----- HÀNH VI DI CHUYỂN (NGOÀI TẦM BẮN) -----
            Vector2D direction = playerPosition - archerTransform.position;
            direction.normalize();
            archerTransform.velocity = direction * m_RepositionSpeed;
        }
    }
}