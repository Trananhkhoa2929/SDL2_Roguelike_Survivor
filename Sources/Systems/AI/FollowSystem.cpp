// File: src/Systems/FollowSystem.cpp (MỚI)
#include "FollowSystem.h"
#include "EntityManager.h"
#include "TransformComponent.h"

void FollowSystem::Update(EntityManager& entityManager, float deltaTime) {
    for (auto& entity : entityManager.GetEntities()) {
        if (entity->HasComponent<TransformComponent>()) {
            auto& transform = entity->GetComponent<TransformComponent>();

            // Nếu thực thể này cần đi theo một thực thể khác
            if (transform.entityToFollow != nullptr) {
                // Kiểm tra xem thực thể cần đi theo có còn tồn tại và có TransformComponent không
                if (transform.entityToFollow->IsActive() && transform.entityToFollow->HasComponent<TransformComponent>()) {
                    auto& targetTransform = transform.entityToFollow->GetComponent<TransformComponent>();
                    // Cập nhật vị trí của thực thể này để nó trùng với tâm của mục tiêu
                    transform.position.x = targetTransform.position.x + (targetTransform.width / 2) - (transform.width / 2);
                    transform.position.y = targetTransform.position.y + (targetTransform.height / 2) - (transform.height / 2);
                }
                else {
                    // Nếu mục tiêu không còn hợp lệ, hủy luôn thực thể đi theo
                    entity->Destroy();
                }
            }
        }
    }
}