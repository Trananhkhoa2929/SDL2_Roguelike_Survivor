// file: TileEffectSystem.cpp
#include "TileEffectSystem.h"
#include "EntityManager.h"
#include "Map.h"
#include "PlayerControllerComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "EventManager.h"  // Để gửi sự kiện khi người chơi bị thương
#include "PlayerEvents.h"  // Để định nghĩa PlayerDamagedEvent

// Có thể định nghĩa các ID của tile ở một nơi chung để dễ quản lý
const int GRASS_TILE = 0;
const int DIRT_TILE = 1;
const int WATER_TILE = 2;
const int LAVA_TILE = 3;

void TileEffectSystem::Update(EntityManager& entityManager, const Map* map, float deltaTime) {
    if (!map) return;

    // Hiện tại chỉ áp dụng cho người chơi, có thể mở rộng cho quái sau
    for (auto& entity : entityManager.GetEntities()) {
        if (!entity->IsActive() || !entity->HasComponent<PlayerControllerComponent>()) {
            continue;
        }

        auto& transform = entity->GetComponent<TransformComponent>();

        // Lấy vị trí trung tâm của entity để kiểm tra
        int checkX = static_cast<int>(transform.position.x + transform.width / 2.0f);
        int checkY = static_cast<int>(transform.position.y + transform.height / 2.0f);

        int tileId = map->GetTileIdAt(checkX, checkY);

        // Sử dụng switch để xử lý các loại địa hình
        switch (tileId) {
        case WATER_TILE: {
            // Hiệu ứng NƯỚC: Giảm 50% tốc độ di chuyển
            transform.velocity = transform.velocity * 0.5f;
            break;
        }

        case LAVA_TILE: {
            // Hiệu ứng LAVA: Mất 5 máu mỗi giây
            m_lavaTimers[entity.get()] += deltaTime;
            if (m_lavaTimers[entity.get()] >= 1.0f) {
                m_lavaTimers[entity.get()] -= 1.0f; // Reset bộ đếm

                if (auto* health = entity->TryGetComponent<HealthComponent>()) {
                    int damageAmount = 5;
                    health->currentHealth -= damageAmount;

                    // Gửi sự kiện để UI hiển thị số sát thương
                    EventManager::GetInstance()->Publish<PlayerDamagedEvent>(entity.get(), damageAmount);
                }
            }
            break;
        }

        default:
            // Đứng trên các địa hình khác (cỏ, đất)
            // Reset bộ đếm dung nham nếu entity không còn đứng trên đó
            if (m_lavaTimers.count(entity.get())) {
                m_lavaTimers[entity.get()] = 0.0f;
            }
            break;
        }
    }
}