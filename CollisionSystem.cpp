#include "CollisionSystem.h"
#include "ColliderComponent.h"
#include "EntityManager.h"
#include "HealthComponent.h"
#include "ProjectileComponent.h"
#include "ScoreComponent.h" // Thêm vào để cộng điểm
#include "PlayerControllerComponent.h" // Thêm vào để tìm người chơi
#include "TransformComponent.h"
#include "TextComponent.h"
#include "DamageTextComponent.h"
#include <utility> // Cho std::pair và std::make_pair
#include <iostream>
#include <string>

bool CollisionSystem::CheckAABBCollision(const SDL_Rect& a, const SDL_Rect& b) {
	return (a.x < b.x + b.w && a.x + a.w > b.x &&
		a.y < b.y + b.h && a.y + a.h > b.y);

}

// REFACTOR: Tách logic xử lý va chạm thành các hàm riêng biệt
void HandlePlayerEnemyCollision(Entity* player, Entity* enemy, EntityManager& entityManager) {
	int damageTaken = 10;

	if (auto* health = player->TryGetComponent<HealthComponent>()) {
		// BUG FIX: Kẻ thù chạm vào người chơi nên gây sát thương nhiều hơn.
		health->currentHealth -= 10;
		std::cout << "Player took 10 damage from enemy collision. Current health: " << health->currentHealth << std::endl;
	}
	if (auto* playerTransform = player->TryGetComponent<TransformComponent>()) {
		Entity& damageText = entityManager.AddEntity();
		std::string text = "-" + std::to_string(damageTaken);
		SDL_Color color = { 255, 50, 50, 255 }; // Màu đỏ tươi

		// Vị trí xuất hiện ở bên phải của player
		float spawnX = playerTransform->position.x + playerTransform->width;
		float spawnY = playerTransform->position.y;

		damageText.AddComponent<TransformComponent>(spawnX, spawnY, 0, 0, 1.0);
		damageText.AddComponent<TextComponent>(text, "main_font", color);
		damageText.AddComponent<DamageTextComponent>();
	}
	// Kẻ thù bị hủy sau khi va chạm.
	enemy->Destroy();

}

void HandleProjectileEnemyCollision(Entity* projectile, Entity* enemy, EntityManager& entityManager) {
	if (auto* health = enemy->TryGetComponent<HealthComponent>()) {
		if (auto* projData = projectile->TryGetComponent<ProjectileComponent>()) {
			int damage = static_cast<int>(projData->damage);
			health->currentHealth -= damage;
			std::cout << "Enemy took " << damage << " damage. New health: " << health->currentHealth << std::endl;
		}
	}
	// Đạn luôn bị hủy sau khi va chạm.
	projectile->Destroy();
}


void CollisionSystem::Update(EntityManager& entityManager)
{
	auto& entities = entityManager.GetEntities();

	// Tối ưu hóa: Tạo một vector chỉ chứa các thực thể có ColliderComponent
	std::vector<Entity*> collidables;
	for (const auto& entity : entities) {
		if (entity->IsActive() && entity->HasComponent<ColliderComponent>()) {
			collidables.push_back(entity.get());
		}
	}

	// Vòng lặp O(n^2) trên các thực thể có thể va chạm
	for (size_t i = 0; i < collidables.size(); ++i)
	{
		for (size_t j = i + 1; j < collidables.size(); ++j)
		{
			Entity* entityA = collidables[i];
			Entity* entityB = collidables[j];

			auto& colliderA = entityA->GetComponent<ColliderComponent>();
			auto& colliderB = entityB->GetComponent<ColliderComponent>();

			// Bỏ qua nếu một trong hai đã bị xử lý trong cùng frame này
			if (!entityA->IsActive() || !entityB->IsActive()) continue;

			if (CheckAABBCollision(colliderA.hitbox, colliderB.hitbox))
			{
				// REFACTOR: Sắp xếp các tag theo thứ tự bảng chữ cái để giảm số lượng trường hợp cần xử lý
				std::string tagA = colliderA.tag;
				std::string tagB = colliderB.tag;
				if (tagA > tagB) {
					std::swap(tagA, tagB);
					std::swap(entityA, entityB);
				}

				// REFACTOR: Sử dụng một chuỗi if/else sạch sẽ hơn
				if (tagA == "enemy" && tagB == "player") {
					HandlePlayerEnemyCollision(entityB, entityA, entityManager);
				}
				else if (tagA == "enemy" && tagB == "projectile") {
					HandleProjectileEnemyCollision(entityB, entityA, entityManager);
				}
				// Thêm các trường hợp va chạm khác ở đây (ví dụ: enemy vs enemy)
			}
		}
	}
}