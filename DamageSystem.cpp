// File: src/Systems/DamageSystem.cpp
#include "DamageSystem.h"
#include "EntityManager.h"
#include "ColliderComponent.h"
#include "WeaponComponents.h"
#include "HealthComponent.h"
#include "ProjectileComponent.h"
#include "TextComponent.h"
#include "DamageTextComponent.h"
#include "TransformComponent.h"
#include <string>
#include <vector>

// Hàm CheckAABBCollision có thể được chuyển vào một file tiện ích chung
bool CheckAABBCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x &&
        a.y < b.y + b.h && a.y + a.h > b.y);
}

void DamageSystem::Update(EntityManager& entityManager, float deltaTime) {
    std::vector<Entity*> weaponHitboxes;
    std::vector<Entity*> enemies;

    // Phân loại các thực thể
    for (auto& entity : entityManager.GetEntities()) {
        if (!entity->IsActive()) continue;
        if (entity->HasComponent<ColliderComponent>()) {
            if (entity->GetComponent<ColliderComponent>().tag == "player_weapon" && entity->HasComponent<HitboxComponent>()) {
                weaponHitboxes.push_back(entity.get());
            }
            else if (entity->GetComponent<ColliderComponent>().tag == "enemy" && entity->HasComponent<HealthComponent>()) {
                enemies.push_back(entity.get());
            }
        }
    }

    // Kiểm tra va chạm
    for (auto& hitbox : weaponHitboxes) {
        if (!hitbox->IsActive()) continue;

        auto& hitboxCollider = hitbox->GetComponent<ColliderComponent>();
        auto& hitboxData = hitbox->GetComponent<HitboxComponent>();

        for (auto& enemy : enemies) {
            if (!enemy->IsActive()) continue;

            auto& enemyCollider = enemy->GetComponent<ColliderComponent>();
            if (CheckAABBCollision(hitboxCollider.hitbox, enemyCollider.hitbox)) {
                auto& enemyHealth = enemy->GetComponent<HealthComponent>();
                enemyHealth.currentHealth -= hitboxData.damage;
                // Có thể thêm logic "đã bị đánh trúng" để tránh một hitbox gây sát thương nhiều lần
                
                // SỬA: TẠO RA SỐ SÁT THƯƠNG KHI ENEMY BỊ TRÚNG ĐÒN
                if (enemy->HasComponent<TransformComponent>()) {
                    auto& enemyTransform = enemy->GetComponent<TransformComponent>();
                    Entity& damageText = entityManager.AddEntity();
                    std::string text = "-" + std::to_string(static_cast<int>(hitboxData.damage));
                    SDL_Color color = { 255, 200, 0, 255 }; // Màu vàng cam

                    // Vị trí xuất hiện ở bên trái của enemy
                    float spawnX = enemyTransform.position.x - 30;
                    float spawnY = enemyTransform.position.y;

                    damageText.AddComponent<TransformComponent>(spawnX, spawnY, 0, 0, 1.0);
                    damageText.AddComponent<TextComponent>(text, "main_font", color);
                    damageText.AddComponent<DamageTextComponent>();
                }
                // Điều này ngăn chặn việc "bắn xuyên".
                // Áp dụng cho các hitbox là đạn, không áp dụng cho hào quang như Tỏi.
                if (hitbox->HasComponent<ProjectileComponent>()) {
                    hitbox->Destroy();
                    // TỐI ƯU HÓA: Khi đạn đã trúng mục tiêu và bị hủy,
                    // nó không cần phải kiểm tra va chạm với các kẻ địch khác nữa.
                    // Thoát khỏi vòng lặp duyệt kẻ địch.
                    break;
                }
            }
        }
    }
}