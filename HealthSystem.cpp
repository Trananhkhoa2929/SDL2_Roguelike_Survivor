#include "HealthComponent.h"
#include "HealthSystem.h"
#include "PlayerControllerComponent.h"
#include "EntityManager.h"
#include "LootDropComponent.h"        // MỚI: Cần biết về loot
#include "ExperienceOrbComponent.h"   // MỚI: Để tạo ra orb
#include "GoldCoinComponent.h"
#include "TransformComponent.h"       // MỚI: Để lấy vị trí
#include "SpriteComponent.h"          // MỚI: Để tạo hình ảnh cho orb
#include "ColliderComponent.h"        // MỚI: Để orb có thể được nhặt
#include "PlayerControllerComponent.h" // MỚI: Cần biết về người chơi
#include "ScoreComponent.h"            // MỚI: Để cập nhật điểm
#include "ScoreValueComponent.h"       // MỚI: Để lấy giá trị điểm
#include <iostream>

void HealthSystem::Update(EntityManager& entityManager, float deltaTime) 
{
    // TỐI ƯU: Tìm ScoreComponent của người chơi MỘT LẦN DUY NHẤT ở đầu hàm,
    // thay vì tìm kiếm nhiều lần bên trong vòng lặp.
    ScoreComponent* playerScoreComponent = nullptr;
    for (auto& entity : entityManager.GetEntities()) {
        if (entity->HasComponent<PlayerControllerComponent>() && entity->HasComponent<ScoreComponent>()) {
            playerScoreComponent = &entity->GetComponent<ScoreComponent>();
            break; // Tìm thấy người chơi thì dừng lại
        }
    }

    // Duyệt qua tất cả các thực thể để tìm những thực thể đã chết
    for (auto& entity : entityManager.GetEntities())
    {
        if (entity->HasComponent<HealthComponent>() && entity->GetComponent<HealthComponent>().currentHealth <= 0)
        {
            // SỬA: Logic cộng điểm khi chết
            if (playerScoreComponent && entity->HasComponent<ScoreValueComponent>())
            {
                auto& scoreValue = entity->GetComponent<ScoreValueComponent>();
                playerScoreComponent->score += scoreValue.scoreValue;
                // Debug để xem điểm có tăng không
                // std::cout << "Player score: " << playerScoreComponent->score << std::endl;
            }

            // Logic tạo vật phẩm rơi ra (giữ nguyên)
            if (entity->HasComponent<LootDropComponent>() && entity->HasComponent<TransformComponent>())
            {
                auto& loot = entity->GetComponent<LootDropComponent>();
                auto& transform = entity->GetComponent<TransformComponent>();
                Entity& xpOrb = entityManager.AddEntity();
                xpOrb.AddComponent<TransformComponent>(transform.position.x, transform.position.y, 16, 16, 1.0);
                xpOrb.AddComponent<SpriteComponent>("xp_orb_sprite");
                xpOrb.AddComponent<ColliderComponent>("loot");
                xpOrb.AddComponent<ExperienceOrbComponent>(loot.xpAmount);

                //MỚI: Logic tạo ra vàng
                if (loot.goldAmount > 0) {
                    Entity& goldCoin = entityManager.AddEntity();
                    goldCoin.AddComponent<TransformComponent>(transform.position.x + 8, transform.position.y + 8, 16, 16, 1.0);
                    goldCoin.AddComponent<SpriteComponent>("gold_coin_sprite"); // Cần load sprite này
                    goldCoin.AddComponent<ColliderComponent>("loot");
                    goldCoin.AddComponent<GoldCoinComponent>(loot.goldAmount);
                }
            }

            // Hủy thực thể sau khi đã xử lý xong
            entity->Destroy();
        }
    }
}