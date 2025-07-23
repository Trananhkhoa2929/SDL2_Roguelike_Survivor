
#include "LootSystem.h"
#include "EntityManager.h"
#include "PlayerControllerComponent.h"
#include "ExperienceOrbComponent.h"
#include "GoldCoinComponent.h"
#include "LevelComponent.h"
#include "TransformComponent.h"

void LootSystem::Update(EntityManager& entityManager, float deltaTime)
{
    // 1. Tìm người chơi và các component cần thiết của họ
    Entity* player = nullptr;
    TransformComponent* playerTransform = nullptr;
    PlayerControllerComponent* playerController = nullptr;
    LevelComponent* playerLevel = nullptr;

    for (auto& entity : entityManager.GetEntities()) {
        if (entity->HasComponent<PlayerControllerComponent>()) {
            player = entity.get();
            playerTransform = &player->GetComponent<TransformComponent>();
            playerController = &player->GetComponent<PlayerControllerComponent>();
            playerLevel = &player->GetComponent<LevelComponent>();
            break;
        }
    }

    // Nếu không có người chơi hoặc người chơi thiếu component, không làm gì cả
    if (!player || !playerLevel || !playerController) return;
    
    // 2. Duyệt qua tất cả các thực thể để tìm các viên XP
    for (auto& entity : entityManager.GetEntities())
    {
        if (!entity->IsActive()) continue;

        if (entity->HasComponent<ExperienceOrbComponent>())
        {
            auto& orbTransform = entity->GetComponent<TransformComponent>();
            float distanceSq = playerTransform->position.distanceSq(orbTransform.position);

            // 3. Kiểm tra tầm nhặt
            if (distanceSq < (playerController->pickupRange * playerController->pickupRange))
            {
                auto& orb = entity->GetComponent<ExperienceOrbComponent>();
                playerLevel->AddXP(orb.xpValue);
                entity->Destroy();
            }
            // 4. Kiểm tra tầm hút
            else if (distanceSq < (playerController->magnetRange * playerController->magnetRange))
            {
                Vector2D direction = playerTransform->position - orbTransform.position;
                direction.normalize();
                // Tốc độ hút có thể được điều chỉnh
                orbTransform.velocity = direction * 500.0f;
            }
            else
            {
                // Nếu ngoài tầm hút, đứng yên
                orbTransform.velocity = { 0.0f, 0.0f };
            }
        }
        // SỬA: Thêm một biến để lưu trữ vàng nhặt được trong lượt chơi
        
         // MỚI: Thêm logic nhặt vàng
        if (entity->HasComponent<GoldCoinComponent>())
        {
            auto& coinTransform = entity->GetComponent<TransformComponent>();
            float distanceSq = playerTransform->position.distanceSq(coinTransform.position);
            if (distanceSq < (playerController->pickupRange * playerController->pickupRange)) {
                playerController->sessionGold += entity->GetComponent<GoldCoinComponent>().value;
                entity->Destroy();
            }
        }
    }
}

